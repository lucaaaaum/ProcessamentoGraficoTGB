#pragma once

#include "processing/filter.h"
#include <QPushButton>
#include <QWidget>

class FilterWidget : public QWidget {
  Q_OBJECT

private:
  int index;
  void setupLayout(const std::string &name);
  void setupConnections();

  QPushButton *removeButton;
  QPushButton *moveUpButton;
  QPushButton *moveDownButton;

public:
  FilterWidget(const Filter &filter, int index, QWidget *parent = nullptr);

signals:
  void removeRequested(int index);
  void moveUpRequested(int index);
  void moveDownRequested(int index);
};
