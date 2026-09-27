#include <opencv2/opencv.hpp>
#include <iostream>
#include "func.h"

using namespace cv;

int main() {

    Mat img = read_origin("resources/image_0.png");
    if (img.empty()) return 1;
    namedWindow("Original", cv::WINDOW_NORMAL);
    imshow("Original", img);
    waitKey(0);

    Mat gray_img = gray(img);

    Mat GI = blur_me(img);
    Mat Mask0 = HSV_red(GI);



    Mat Mask = refine_mask(Mask0);
    Mat sure_fg = threshold_mask(Mask);
    namedWindow("Mask", WINDOW_NORMAL);
    imshow("Mask", sure_fg);
    waitKey(0);



    Mat draw = img.clone();


    Mat drawn_1 = select_1(sure_fg, draw);

    namedWindow("Recognition", WINDOW_NORMAL);
    imshow("Recognition", drawn_1);
    waitKey(0);

    

    Mat sobel_img = sobel_me(gray_img);
    namedWindow("Sobel", WINDOW_NORMAL);
    imshow("Sobel", sobel_img);
    waitKey(0);

    Mat equalized_img = equalizeHist_me(gray_img);
    namedWindow("Equalized", WINDOW_NORMAL);
    imshow("Equalized", equalized_img);
    waitKey(0);



    destroyAllWindows(); 
    return 0;
}