#pragma once
#include <functional>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

class Filter {
private:
  char *name;
  std::function<void(cv::Mat &)> filterFunction;

public:
  Filter(char *name, std::function<void(cv::Mat &)> func)
      : name(name), filterFunction(func) {}
  void apply(cv::Mat &image);

  static Filter grayScaleFilter() {
    return Filter("Gray Scale", [](cv::Mat &image) {
      cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
      cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    });
  }
};
