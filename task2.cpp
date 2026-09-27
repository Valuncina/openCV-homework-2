// angular_velocity.cpp
// 追踪视频中旋转块，拟合 omega(t) = a + b*sin(c*t + d)
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <ceres/ceres.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Residual {
    double x_, y_;
    Residual(double x, double y) : x_(x), y_(y) {}
    template <typename T>
    bool operator()(const T* const abcd, T* residual) const {
        residual[0] = T(y_) - abcd[0] - abcd[1] * ceres::sin(abcd[2] * T(x_) + abcd[3]);
        return true;
    }
};

// 求旋转中心
static void findRotationCenter(const std::vector<cv::Point2f>& pts, double& cx, double& cy) {
    int n = (int)pts.size();
    cv::Mat A(n, 3, CV_64F), B(n, 1, CV_64F);
    for (int i = 0; i < n; i++) {
        double x = pts[i].x, y = pts[i].y;
        A.at<double>(i,0) = x;  A.at<double>(i,1) = y;  A.at<double>(i,2) = 1.0;
        B.at<double>(i,0) = -(x*x + y*y);
    }
    cv::Mat sol;
    cv::solve(A, B, sol, cv::DECOMP_SVD);
    cx = -sol.at<double>(0,0) / 2.0;
    cy = -sol.at<double>(1,0) / 2.0;
}

//ai优化的，防止局部最优解
static double linearFitForGivenC(double c,
                                  const std::vector<double>& t,
                                  const std::vector<double>& w,
                                  double& a, double& B1, double& B2) {
    // 正规方程 3x3
    double S00=0, S10=0, S20=0, S11=0, S12=0, S22=0;
    double z0=0, z1=0, z2=0;
    int n = (int)t.size();
    for (int i = 0; i < n; i++) {
        double s = std::sin(c*t[i]);
        double q = std::cos(c*t[i]);
        double y = w[i];
        S00 += 1;     S10 += s;      S20 += q;
                      S11 += s*s;    S12 += s*q;
                                     S22 += q*q;
        z0 += y;  z1 += s*y;  z2 += q*y;
    }
    // 列主元高斯消元 (3x3)
    double m[3][4] = {
        {S00, S10, S20, z0},
        {S10, S11, S12, z1},
        {S20, S12, S22, z2}
    };
    for (int col = 0; col < 3; col++) {
        int piv = col;
        for (int r = col+1; r < 3; r++) if (std::abs(m[r][col]) > std::abs(m[piv][col])) piv = r;
        if (piv != col) std::swap_ranges(m[col], m[col]+4, m[piv]);
        for (int r = 0; r < 3; r++) if (r != col) {
            double f = m[r][col]/m[col][col];
            for (int k = col; k < 4; k++) m[r][k] -= f*m[col][k];
        }
    }
    a  = m[0][3]/m[0][0];
    B1 = m[1][3]/m[1][1];
    B2 = m[2][3]/m[2][2];
    double sse = 0;
    for (int i = 0; i < n; i++) {
        double pred = a + B1*std::sin(c*t[i]) + B2*std::cos(c*t[i]);
        double e = w[i] - pred;
        sse += e*e;
    }
    return sse;
}


int main(int argc, char** argv) {
    std::string videoPath = (argc > 1) ? argv[1] : "resources/task_2.mp4";
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) { std::cerr << "cannot open: " << videoPath << std::endl; return -1; }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int W = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int H = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "video " << W << "x" << H << " @ " << fps << " fps" << std::endl;

    std::vector<cv::Point2f> centers;
    std::vector<double> timestamps;

    cv::Mat frame;
    int fi = 0;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        fi++;
        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::Mat mask;
        cv::inRange(hsv, cv::Scalar(90, 80, 80), cv::Scalar(135, 255, 255), mask);
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // 面积最大
        int best = -1; double bestArea = 0;
        for (int i = 0; i < (int)contours.size(); i++) {
            double a = cv::contourArea(contours[i]);
            if (a > bestArea) { bestArea = a; best = i; }
        }
        if (best >= 0 && bestArea > 20) {
            cv::Rect r = cv::boundingRect(contours[best]);
            centers.push_back(cv::Point2f(r.x + r.width/2.0f, r.y + r.height/2.0f));
            timestamps.push_back(fi / fps);
        }
    }
    cap.release();
    std::cout << "detected " << centers.size() << " frames" << std::endl;
    if (centers.size() < 10) { std::cerr << "blob not detected, check HSV range\n"; return -1; }
    //圆心
    double cx, cy;
    findRotationCenter(centers, cx, cy);
    std::cout << "rotation center = (" << cx << ", " << cy << ")" << std::endl;

    // theta
    std::vector<double> theta;
    for (size_t i = 0; i < centers.size(); i++) {
        double th = std::atan2(centers[i].y - cy, centers[i].x - cx);
        if (i > 0) {
            double d = th - theta.back();
            while (d >  M_PI) { th -= 2*M_PI; d = th - theta.back(); }
            while (d < -M_PI) { th += 2*M_PI; d = th - theta.back(); }
        }
        theta.push_back(th);
    }

    // 差分
    std::vector<double> tW, wW;
    for (size_t i = 0; i + 1 < theta.size(); i++) {
        double dt = timestamps[i+1] - timestamps[i];
        tW.push_back(0.5*(timestamps[i] + timestamps[i+1]));
        wW.push_back((theta[i+1] - theta[i]) / dt);
    }

    // ai优化初始值
    double bestC = 1.0, bestSSE = 1e30;
    double a0=0, B10=0, B20=0;
    for (double c = 0.1; c <= 6.0; c += 0.005) {
        double a, B1, B2;
        double sse = linearFitForGivenC(c, tW, wW, a, B1, B2);
        if (sse < bestSSE) { bestSSE = sse; bestC = c; a0 = a; B10 = B1; B20 = B2; }
    }
    double b0 = std::sqrt(B10*B10 + B20*B20);
    double d0 = std::atan2(B20, B10);   // B1 sin(ct) + B2 cos(ct) = b sin(ct + d)
    std::cout << "coarse init: a=" << a0 << " b=" << b0 << " c=" << bestC << " d=" << d0 << std::endl;

    // 开始拟合
    double abcd[4] = {a0, b0, bestC, d0};
    ceres::Problem problem;
    for (size_t i = 0; i < tW.size(); i++) {
        problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<Residual, 1, 4>(new Residual(tW[i], wW[i])),
            new ceres::HuberLoss(1.0),
            abcd);
    }
    ceres::Solver::Options opt;
    opt.linear_solver_type = ceres::DENSE_QR;
    opt.max_num_iterations = 200;
    opt.minimizer_progress_to_stdout = false;
    ceres::Solver::Summary sum;
    ceres::Solve(opt, &problem, &sum);
    std::cout << sum.BriefReport() << std::endl;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "\n=== omega(t) = a + b sin(c t + d) ===" << std::endl;
    std::cout << "a = " << abcd[0] << " rad/s" << std::endl;
    std::cout << "b = " << abcd[1] << " rad/s" << std::endl;
    std::cout << "c = " << abcd[2] << " rad/s  (T = " << 2*M_PI/abcd[2] << " s)" << std::endl;
    std::cout << "d = " << abcd[3] << " rad"  << std::endl;
    return 0;
}
