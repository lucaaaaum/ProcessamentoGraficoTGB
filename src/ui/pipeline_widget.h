#pragma once

#include "processing/pipeline.h"
#include <QVBoxLayout>
#include <QWidget>

class PipelineWidget : public QWidget {
  Q_OBJECT

private:
  QVBoxLayout *layout;
  Pipeline *pipeline;

public:
  PipelineWidget(Pipeline *pipeline, QWidget *parent = nullptr);
};
