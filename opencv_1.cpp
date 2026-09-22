#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    Mat img = imread("image_0.png");
    if (img.empty()) {
        std::cerr << "Cannot read image! 请检查图片路径\n";
        return 1;
    }

    namedWindow("Original", WINDOW_NORMAL);
    imshow("Original", img);
    // 按键盘任意键继续
    waitKey(0);

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    
    if (!gray.empty()) { // 增加判空，防止崩溃
        namedWindow("Gray", WINDOW_NORMAL);
        imshow("Gray", gray);
    }
    
    waitKey(0);


    Mat meanImg, gaussianImg, medianImg;
    namedWindow("Mean", WINDOW_NORMAL);
    namedWindow("Gaussian", WINDOW_NORMAL);
    namedWindow("Median", WINDOW_NORMAL);
    blur(img, meanImg, Size(5, 5));
    GaussianBlur(img, gaussianImg, Size(5, 5), 1.5);
    medianBlur(img, medianImg, 5);
    imshow("Mean", meanImg);
    waitKey(0);
    imshow("Gaussian", gaussianImg);
    waitKey(0);
    imshow("Median", medianImg);
    waitKey(0);
    
    destroyAllWindows(); // 主动释放所有窗口资源，比直接点叉优雅
    return 0;
}