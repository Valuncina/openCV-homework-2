#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <ceres/ceres.h>
#include "../common/func.h"

using namespace cv;

int main(int argc, char** argv) {

    std::string videoPath = (argc > 1) ? argv[1] : "resources/task_3.mp4";
    VideoCapture cap(videoPath);
    Mat frame;


    int codec = VideoWriter::fourcc('a', 'v', 'c', '1');
    int frame_width  = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    
    
    std::string outputPath = "output.mp4";                    
    VideoWriter writer(outputPath, codec, fps, Size(frame_width, frame_height), true);
    if (!writer.isOpened()) {
        std::cerr << "无法创建输出视频: " << outputPath << std::endl;
        return -1;
    }   
    
    while (true)
    {
        cap >> frame;
        if (frame.empty()) break; 
        Mat hsv, Mask, Mask_bgr;
        Mat draw = frame.clone();
        cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        inRange(hsv, Scalar(0,   70, 50), Scalar(20, 255, 255), Mask);

        Mat finale = adaptive_me(Mask);
        //contours
        std::vector<std::vector<Point>> contours;
        findContours(finale, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        for (size_t i=0; i<contours.size(); i++) {

            double area = contourArea(contours[i]);
            Rect rect = boundingRect(contours[i]);
            if (rect.height == 0) continue;
            double aspect_ratio = (double)rect.width / rect.height;
            if (!(aspect_ratio > 0.3 && aspect_ratio < 2.0 && area > 70)) continue;
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

        writer.write(draw);
    
        
    }
    
    writer.release();
    cap.release();
    return 0;
}