#pragma once
#include <functional>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

class Filter {
private:
  std::string name;
  std::function<void(cv::Mat &)> filterFunction;

public:
  Filter(std::string name, std::function<void(cv::Mat &)> func)
      : name(name), filterFunction(func) {}

  void apply(cv::Mat &image);

  std::string getName() const { return name; }

  static std::vector<Filter> presets() {
    return {grayScaleFilter(), blurFilter(),      boxBlurFilter(),
            medianFilter(),    sharpenFilter(),   laplacianFilter(),
            cannyFilter(),     negativeFilter(),  thresholdFilter(),
            brightnessContrastFilter()};
  }

  static Filter grayScaleFilter() {
    return Filter("Gray Scale", [](cv::Mat &image) {
      cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
      cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    });
  }

  static Filter blurFilter() {
    return Filter("Blur", [](cv::Mat &image) {
      cv::GaussianBlur(image, image, cv::Size(5, 5), 0);
    });
  }

  static Filter boxBlurFilter() {
    return Filter("Box Blur", [](cv::Mat &image) {
      cv::blur(image, image, cv::Size(5, 5));
    });
  }

  static Filter medianFilter() {
    return Filter("Mediana", [](cv::Mat &image) {
      cv::medianBlur(image, image, 5);
    });
  }

  static Filter sharpenFilter() {
    return Filter("Sharpen", [](cv::Mat &image) {
      cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
      cv::filter2D(image, image, -1, kernel);
    });
  }

  static Filter laplacianFilter() {
    return Filter("Laplaciano", [](cv::Mat &image) {
      cv::Mat gray, laplacian;
      cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
      cv::Laplacian(gray, laplacian, CV_16S, 3);
      cv::convertScaleAbs(laplacian, gray);
      cv::cvtColor(gray, image, cv::COLOR_GRAY2BGR);
    });
  }

  static Filter cannyFilter() {
    return Filter("Canny", [](cv::Mat &image) {
      cv::Mat gray, edges;
      cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
      cv::Canny(gray, edges, 100, 200);
      cv::cvtColor(edges, image, cv::COLOR_GRAY2BGR);
    });
  }

  static Filter negativeFilter() {
    return Filter("Negativo",
                  [](cv::Mat &image) { cv::bitwise_not(image, image); });
  }

  static Filter thresholdFilter() {
    return Filter("Limiarização", [](cv::Mat &image) {
      cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
      cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);
      cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    });
  }

  static Filter brightnessContrastFilter() {
    return Filter("Brilho/Contraste", [](cv::Mat &image) {
      image.convertTo(image, -1, 1.3, 20);
    });
  }
};
