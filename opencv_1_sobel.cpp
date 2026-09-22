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

    // Sobel 边缘检测
    Mat gradX, gradY, absX, absY, grad;
    
    // 计算 X 和 Y 方向梯度（必须用 CV_16S 保存负数）
    Sobel(gray, gradX, CV_16S, 1, 0);
    Sobel(gray, gradY, CV_16S, 0, 1);
    
    // 取绝对值并转回 8 位（CV_8U）用于显示
    convertScaleAbs(gradX, absX);
    convertScaleAbs(gradY, absY);
    
    // 融合 X 和 Y 方向的边缘
    addWeighted(absX, 0.5, absY, 0.5, 0, grad);

    // 4. 显示 Sobel 结果（如果图片太大，记得先用 namedWindow 调大小）
    namedWindow("Sobel", WINDOW_NORMAL);
    imshow("Sobel", grad);
    

    waitKey(0);         // 最后停住，防止窗口闪退
    destroyAllWindows();
    return 0;
}