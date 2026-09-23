#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    Mat img = imread("resources/image_0.png");
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


    Mat binary , adaptive;
    threshold(gray, binary, 128, 255, THRESH_BINARY);
    GaussianBlur(gray, gray, Size(7, 7), 2.0); 
    //先高斯去噪，防止噪声影响自适应阈值分割
    adaptiveThreshold(gray, adaptive, 255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
    morphologyEx(adaptive, adaptive, MORPH_CLOSE, kernel); 
    // 填补内部黑点

    namedWindow("Binary",WINDOW_NORMAL);
    imshow("Binary",binary);
    namedWindow("Adaptive",WINDOW_NORMAL);
    imshow("Adaptive",adaptive);
    waitKey(0);
    
    destroyAllWindows(); // 主动释放所有窗口资源，比直接点叉优雅
    return 0;
}