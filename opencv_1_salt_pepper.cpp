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
    // 按键盘任意键继续（不要点窗口的叉！）
    waitKey(0);

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    
    if (!gray.empty()) { // 增加判空，防止崩溃
        namedWindow("Gray", WINDOW_NORMAL);
        imshow("Gray", gray);
    }
    
    waitKey(0);


    // 1. 复制原图作为噪声图
    Mat noiseImg = img.clone();
    namedWindow("Noise Image", WINDOW_NORMAL);

    // 2. 生成椒盐噪声（大概 5% 的像素变成黑点或白点）
    int noiseNum = noiseImg.rows * noiseImg.cols * 0.05; // 噪声点数量
    for (int i = 0; i < noiseNum; i++) {
        int row = rand() % noiseImg.rows;
        int col = rand() % noiseImg.cols;
        // 一半变成盐（白），一半变成椒（黑）
        if (i % 2 == 0) {
            noiseImg.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
        } else {
            noiseImg.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
        }
    }
    imshow("Noise Image", noiseImg);

    waitKey(0);

    Mat meanImg, gaussianImg, medianImg;
    namedWindow("Mean", WINDOW_NORMAL);
    namedWindow("Gaussian", WINDOW_NORMAL);
    namedWindow("Median", WINDOW_NORMAL);
    blur(noiseImg, meanImg, Size(5, 5));
    GaussianBlur(noiseImg, gaussianImg, Size(5, 5), 1.5);
    medianBlur(noiseImg, medianImg, 5);
    imshow("Mean", meanImg);
    waitKey(0);
    imshow("Gaussian", gaussianImg);
    waitKey(0);
    imshow("Median", medianImg);
    waitKey(0);
    
    destroyAllWindows(); // 主动释放所有窗口资源，比直接点叉优雅
    return 0;
}