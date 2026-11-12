#include "camera.h"
#include <opencv2/videoio.hpp>
#include <stdexcept>

Camera::Camera(int deviceIndex) : capture(deviceIndex, cv::CAP_V4L2) {
  if (!capture.isOpened()) {
    throw std::runtime_error("Erro ao abrir a câmera.");
  }
}

void Camera::start() {
  capture.open(0, cv::CAP_V4L2);
  timer.start(30);
  connect(&timer, &QTimer::timeout, this, [this]() {
    cv::Mat frame;
    capture >> frame;
    if (!frame.empty()) {
      emit frameChanged(frame);
    }
  });
}

void Camera::stop() {
  timer.stop();
  capture.release();
}
