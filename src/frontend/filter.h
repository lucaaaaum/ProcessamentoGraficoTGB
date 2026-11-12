#pragma once
#include <functional>
#include <opencv2/core/mat.hpp>

class Filter {
private:
  std::function<void(cv::Mat &)> filterFunction;

public:
  Filter(std::function<void(cv::Mat &)> func) : filterFunction(func) {}
  void apply(cv::Mat &image);
};
