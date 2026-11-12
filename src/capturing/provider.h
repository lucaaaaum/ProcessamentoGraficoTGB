#pragma once

#include <QObject>
#include <opencv2/core/mat.hpp>

class Provider : public QObject {
  Q_OBJECT

public:
  using QObject::QObject;
  virtual void start() = 0;
  virtual void stop() = 0;

signals:
  void frameChanged(const cv::Mat &frame);
};
