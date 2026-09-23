#include <opencv2/opencv.hpp>
#include <iostream>
#include "read_origin.h"
    
using namespace cv;

Mat read_origin(const std::string& filename) {
    Mat img = imread(filename);
    if (img.empty()) {
        std::cerr << "Cannot read image! 请检查图片路径\n";
        return Mat();  // 返回空矩阵
    }

    namedWindow("Original", WINDOW_NORMAL);
    imshow("Original", img);
    // 按键盘任意键继续
    waitKey(0);
    return img;
}