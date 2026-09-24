// func.h
#ifndef FUNC_H
#define FUNC_H

#include <string>
#include <opencv2/opencv.hpp>

cv::Mat read_origin(const std::string& filename);

// 提取红色区域掩膜（输入 BGR 图，输出单通道 Mask）
cv::Mat HSV_red(const cv::Mat& img);

cv::Mat refine_mask(cv::Mat& mask0);

cv::Mat threshold_mask(const cv::Mat& Mask);

cv::Mat select_red_regions(const cv::Mat& sure_fg, const cv::Mat& draw);

cv::Mat gray(const cv::Mat& img);

cv::Mat sobel_me(const cv::Mat& gray);

cv::Mat equalizeHist_me(const cv::Mat& gray);

cv::Mat binary_me(const cv::Mat& gray);

cv::Mat canny_me(const cv::Mat& gray);

cv::Mat adaptive_me(const cv::Mat& gray);

cv::Mat blur_me(const cv::Mat& img);

#endif