#pragma once

#include "provider.h"
#include <QImage>

class Image : public Provider {
private:
  cv::Mat image;

public:
  explicit Image(const cv::Mat &img) : image(img) {}
  void start() override;
  void stop() override;
};
