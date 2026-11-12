#pragma once

#include "capturing/provider.h"
#include "processing/pipeline.h"
#include <QObject>
#include <opencv2/imgproc.hpp>

class Controller : public QObject {
  Q_OBJECT

private:
  Provider *provider;
  Pipeline *pipeline;
  QMetaObject::Connection connection;

public:
  using QObject::QObject;
  Controller(QObject *parent = nullptr)
      : QObject(parent), provider(nullptr), pipeline(new Pipeline) {
    pipeline->addFilter(Filter([](cv::Mat &image) {
      cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
      cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    }));
  }
  void setProvider(Provider *prov);
  void addFilter(const Filter &filter);

signals:
  void frameChanged(const cv::Mat &frame);
};
