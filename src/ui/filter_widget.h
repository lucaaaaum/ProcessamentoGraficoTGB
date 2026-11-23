#pragma once

#include "processing/filter.h"
#include <QCheckBox>
#include <QPushButton>
#include <QWidget>

class FilterWidget : public QWidget {
  Q_OBJECT

private:
  int index;
  void setupLayout(const Filter &filter);
  void setupConnections();
  void onChannelToggled(QCheckBox *checkBox);
  Channel selectedChannels() const;

  QPushButton *removeButton;
  QPushButton *moveUpButton;
  QPushButton *moveDownButton;
  QCheckBox *redCheckBox;
  QCheckBox *greenCheckBox;
  QCheckBox *blueCheckBox;

public:
  FilterWidget(const Filter &filter, int index, QWidget *parent = nullptr);

signals:
  void removeRequested(int index);
  void moveUpRequested(int index);
  void moveDownRequested(int index);
  void channelsChanged(int index, Channel channels);
};
