// read_origin.cpp
#include "func.h"
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

Mat read_origin(const std::string& filename) {
    Mat img = imread(filename);
    if (img.empty()) {
        std::cerr << "Cannot read image! 请检查图片路径: " << filename << "\n";
        return cv::Mat();
    }
    return img;
}

// ===== 新增：红色掩膜提取 =====
Mat HSV_red(const Mat& img) {
    if (img.empty()) {
        std::cerr << "extract_red_mask: 输入为空\n";
        return Mat();
    }

    Mat hsv, Masklow, Maskhigh, Mask0;
    cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // 红色跨越色相两端，需要用两段范围
    inRange(hsv, Scalar(0,   110, 90), Scalar(9,   255, 255), Masklow);
    inRange(hsv, Scalar(160, 110, 90), Scalar(179, 255, 255), Maskhigh);
    bitwise_or(Masklow, Maskhigh, Mask0);

    return Mask0;
}

Mat refine_mask(Mat& mask0) {
    if (mask0.empty()) {
        std::cerr << "refine_mask: 输入为空\n";
        return Mat();
    }

    Mat mask, mid;

    // 闭运算：填补小孔洞
    Mat kernel_close = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    morphologyEx(mask0, mid, MORPH_CLOSE, kernel_close, Point(-1, -1), 1);

    // 开运算：去除噪点
    Mat kernel_open = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
    morphologyEx(mid, mask, MORPH_OPEN, kernel_open, Point(-1, -1), 3);

    return mask;
}


Mat threshold_mask(const Mat& Mask) {
    Mat dist;
    distanceTransform(Mask, dist, DIST_L2, 3);
    normalize(dist, dist, 0, 255, NORM_MINMAX); // 归一化到 0~255

    // 阈值化，只保留最高峰（种子点）
    Mat sure_fg;
    threshold(dist, sure_fg, 110, 255, THRESH_BINARY); 
    sure_fg.convertTo(sure_fg, CV_8U);

    return sure_fg;
}


Mat select_red_regions(const Mat& sure_fg, const Mat& draw) {
    std::vector<std::vector<Point>> contours_seed;
    findContours(sure_fg, contours_seed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (size_t i=0; i<contours_seed.size(); i++) {

        double area = contourArea(contours_seed[i]);
        Rect rect = boundingRect(contours_seed[i]);
        if (rect.height == 0) continue;
        double aspect_ratio = (double)rect.width / rect.height;
        if (!(aspect_ratio > 0.2 && aspect_ratio < 2.0 && area > 100)) continue;
        //形状、面积筛选

         int padding = 15; 
        rect.x -= padding;
        rect.y -= padding;
        rect.width += 2 * padding;
        rect.height += 2 * padding;

        rectangle(draw, rect, cv::Scalar(0, 255, 0), 2); 
        cv::Point center(rect.x + rect.width / 2, rect.y + rect.height / 2);
        cv::circle(draw, center, 4, cv::Scalar(0, 0, 255), -1);

    }

    return draw;

}


Mat gray(const Mat& img) {
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    return gray;
}


Mat sobel_me(const Mat& gray) {
    Mat gradX, gradY, absX, absY, grad;
    
    // 计算 X 和 Y 方向梯度（必须用 CV_16S 保存负数）
    Sobel(gray, gradX, CV_16S, 1, 0);
    Sobel(gray, gradY, CV_16S, 0, 1);
    
    // 取绝对值并转回 8 位（CV_8U）用于显示
    convertScaleAbs(gradX, absX);
    convertScaleAbs(gradY, absY);
    
    // 融合 X 和 Y 方向的边缘
    addWeighted(absX, 0.5, absY, 0.5, 0, grad);

    return grad;
}


Mat equalizeHist_me(const Mat& gray) {
    Mat equalized;
    equalizeHist(gray, equalized);
    return equalized;
}


Mat canny_me(const Mat& gray) {
    Mat smooth, cannyImg;
    GaussianBlur(gray, smooth, Size(5,5), 1.5);
    Canny(smooth, cannyImg, 30, 90);
    return cannyImg;
}

Mat binary_me(const Mat& gray) {
    Mat binary;
    threshold(gray, binary, 128, 255, THRESH_BINARY);
    
    return binary;
}

Mat adaptive_me(const Mat& gray) {
    Mat adaptive;
    GaussianBlur(gray, gray, Size(7, 7), 2.0); 
    //先高斯去噪，防止噪声影响自适应阈值分割
    adaptiveThreshold(gray, adaptive, 255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
    morphologyEx(adaptive, adaptive, MORPH_CLOSE, kernel); 
    // 填补内部黑点
    return adaptive;

}