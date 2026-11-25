#pragma once

#include <opencv2/core/mat.hpp>

struct PlacedSticker {
  cv::Mat image;
  cv::Point center;
};

void overlaySticker(cv::Mat &frame, const cv::Mat &sticker, cv::Point center);
