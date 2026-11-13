#pragma once

#include "processing/filter.h"
#include <QPushButton>
#include <QWidget>

class FilterWidget : public QWidget {
  Q_OBJECT

private:
  Filter *filter;
  void setupLayout();
  void setupConnections();

  QPushButton *removeButton;
  QPushButton *moveUpButton;
  QPushButton *moveDownButton;

public:
  FilterWidget(Filter *filter, QWidget *parent = nullptr);

  Filter *getFilter() const;
};
