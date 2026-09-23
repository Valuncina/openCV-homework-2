#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {

    Mat img = imread("resources/image_0.png");
    if (img.empty()){
        std::cerr << "Cannot read image! 请检查图片路径\n";
        return 1;

    }
    
    namedWindow("Original", WINDOW_NORMAL);
    imshow("Original", img);
    waitKey(0);
    
    Mat hsv, Maskhigh, Masklow, Mask;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    // Red spans both ends of the hue spectrum, so we need two ranges to capture it
    inRange(hsv, Scalar(0, 100, 100), Scalar(10,255, 255), Masklow);
    inRange(hsv, Scalar(160, 100, 100), Scalar(179, 255, 255), Maskhigh);
    bitwise_or(Masklow, Maskhigh, Mask);

    Mat kernal = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
    morphologyEx(Mask, Mask, MORPH_CLOSE, kernal);
    morphologyEx(Mask, Mask, MORPH_OPEN, kernal);

    namedWindow("Red Mask", WINDOW_NORMAL);
    imshow("Red Mask", Mask);
    waitKey(0);

    Mat Eq, gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    namedWindow("Gray", WINDOW_NORMAL);
    imshow("Gray", gray);
    waitKey(0);

    equalizeHist(gray, Eq);
    namedWindow("Equalized", WINDOW_NORMAL);
    imshow("Equalized", Eq);
    waitKey(0);

    return 0;
}