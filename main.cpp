#include <opencv2/opencv.hpp>
#include <iostream>
#include "read_origin.h"

using namespace cv;

int main() {
    Mat img = read_origin("resources/image_0.png");
    

    Mat hsv, Maskhigh, Masklow, Mask0, Mask;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    // Red spans both ends of the hue spectrum, so we need two ranges to capture it
    inRange(hsv, Scalar(0, 110, 90), Scalar(9,255, 255), Masklow);
    inRange(hsv, Scalar(160, 110, 90), Scalar(179, 255, 255), Maskhigh);
    bitwise_or(Masklow, Maskhigh, Mask0);

    namedWindow("Mask0", WINDOW_NORMAL);
    imshow("Mask0", Mask0);
    waitKey(0);
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    morphologyEx(Mask0, Mask0, MORPH_CLOSE, kernel, Point(-1,-1), 1); // 闭运算填补小孔洞
    Mat kernel_1 = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
    morphologyEx(Mask0, Mask, MORPH_OPEN, kernel_1, Point(-1,-1), 3); // 开运算去除噪点

    namedWindow("Mask", WINDOW_NORMAL);
    imshow("Mask", Mask);
    waitKey(0);
  
    Mat dist;
    distanceTransform(Mask, dist, DIST_L2, 3);
    normalize(dist, dist, 0, 255, NORM_MINMAX); // 归一化到 0~255

    // 阈值化，只保留最高峰（种子点）
    Mat sure_fg;
    threshold(dist, sure_fg, 110, 255, THRESH_BINARY); 
    sure_fg.convertTo(sure_fg, CV_8U);

    namedWindow("sure_fg", WINDOW_NORMAL);
    imshow("sure_fg", sure_fg);
    waitKey(0);




    // 1. 把距离变换的种子点重新“膨胀”回花朵大小
    //Mat kernel_dilate = getStructuringElement(MORPH_ELLIPSE, Size(25, 25)); 
    //dilate(sure_fg, sure_fg, kernel_dilate); 
    // 2. 然后再找轮廓

    std::vector<std::vector<Point>> contours_seed;

    findContours(sure_fg, contours_seed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 此时你用 contours_seed 去找 boundingRect，就不会有粘连了 
  
    Mat draw = img.clone();

    //Mat contourImg = Mask.clone();
    //std::vector<std::vector<Point>> contours;
    //findContours(contourImg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    

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

    namedWindow("draw", WINDOW_NORMAL);
    imshow("draw", draw);
    waitKey(0);



    destroyAllWindows(); // 主动释放所有窗口资源，比直接点叉优雅
    return 0;
}