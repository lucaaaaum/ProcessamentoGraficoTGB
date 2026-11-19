#pragma once

#include "processing/filter.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class PipelineWidget : public QWidget {
  Q_OBJECT

private:
  QVBoxLayout *layout;
  QVBoxLayout *filtersLayout;
  QPushButton *addFilterButton;

public:
  PipelineWidget(QWidget *parent = nullptr);
  void setFilters(const std::vector<Filter> &filters);

signals:
  void addFilterRequested();
};
