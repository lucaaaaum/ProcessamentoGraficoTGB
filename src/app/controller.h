#pragma once

#include "capturing/provider.h"
#include "processing/pipeline.h"
#include <QObject>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

class Controller : public QObject {
  Q_OBJECT

private:
  Provider *provider;
  Pipeline *pipeline;
  QMetaObject::Connection connection;
  cv::Mat lastFrame;
  void render();

public:
  using QObject::QObject;
  Controller(QObject *parent = nullptr);
  void setProvider(Provider *prov);
  void cleanProvider();
  void addFilter(const Filter &filter);
  std::vector<Filter> listPipelineFilters() const;

signals:
  void frameChanged(const cv::Mat &frame);
  void frameCleared();
};
