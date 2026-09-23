#include <opencv2/opencv.hpp>
#include <iostream>
#include "func.h"

using namespace cv;

int main() {
    // Mat img = read_origin("resources/image_0.png");
    // if (img.empty()) return 1;
    // namedWindow("Original", cv::WINDOW_NORMAL);
    // imshow("Original", img);
    // waitKey(0);

    // Mat Mask0 = HSV_red(img);
    // Mat Mask = refine_mask(Mask0);
    // Mat sure_fg = threshold_mask(Mask);
    // namedWindow("Mask", WINDOW_NORMAL);
    // imshow("Mask", sure_fg);
    // waitKey(0);

    // Mat draw = img.clone();
    // Mat drawn = select_red_regions(sure_fg, draw);
    // namedWindow("draw", WINDOW_NORMAL);
    // imshow("draw", drawn);
    // waitKey(0);

    // Mat gray_img = gray(img);

    // Mat sobel_img = sobel_me(gray_img);
    // namedWindow("Sobel", WINDOW_NORMAL);
    // imshow("Sobel", sobel_img);
    // waitKey(0);

    //  Mat equalized_img = equalizeHist_me(gray_img);
    //  namedWindow("Equalized", WINDOW_NORMAL);
    //  imshow("Equalized", equalized_img);
    //  waitKey(0);



    destroyAllWindows(); // 主动释放所有窗口资源，比直接点叉优雅
    return 0;
}