#pragma once

#include "provider.h"
#include <QTimer>
#include <opencv2/videoio.hpp>

class Camera : public Provider {
private:
  cv::VideoCapture capture;
  QTimer timer;

public:
  explicit Camera(int deviceIndex = 0);
  void start() override;
  void stop() override;
};
