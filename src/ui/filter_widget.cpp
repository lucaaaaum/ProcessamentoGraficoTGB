#include "filter_widget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSignalBlocker>
#include <QVBoxLayout>

FilterWidget::FilterWidget(const Filter &filter, int index, QWidget *parent)
    : QWidget(parent), index(index) {
  setupLayout(filter);
  setupConnections();
}

void FilterWidget::setupLayout(const Filter &filter) {
  auto *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  auto *header = new QHBoxLayout();
  layout->addLayout(header);

  moveUpButton = new QPushButton("↑");
  moveDownButton = new QPushButton("↓");
  removeButton = new QPushButton("✕");

  QString title = QString::fromStdString(filter.getName());
  if (filter.requiresOperand()) {
    title += QString(" (%1)").arg(
        QString::fromStdString(filter.getOperandName()));
  }
  header->addWidget(new QLabel(title), 1);
  header->addWidget(moveUpButton);
  header->addWidget(moveDownButton);
  header->addWidget(removeButton);

  auto *channelsPanel = new QWidget;
  auto *channelsRow = new QHBoxLayout(channelsPanel);
  channelsRow->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(channelsPanel);
  channelsPanel->setVisible(filter.supportsChannelSelection());

  redCheckBox = new QCheckBox("R");
  greenCheckBox = new QCheckBox("G");
  blueCheckBox = new QCheckBox("B");

  Channel channels = filter.getChannels();
  redCheckBox->setChecked(hasFlag(channels, Channel::Red));
  greenCheckBox->setChecked(hasFlag(channels, Channel::Green));
  blueCheckBox->setChecked(hasFlag(channels, Channel::Blue));

  channelsRow->addWidget(new QLabel("Canais:"));
  channelsRow->addWidget(redCheckBox);
  channelsRow->addWidget(greenCheckBox);
  channelsRow->addWidget(blueCheckBox);
  channelsRow->addStretch();
}

void FilterWidget::setupConnections() {
  connect(removeButton, &QPushButton::clicked, this,
          [this]() { emit removeRequested(index); });
  connect(moveUpButton, &QPushButton::clicked, this,
          [this]() { emit moveUpRequested(index); });
  connect(moveDownButton, &QPushButton::clicked, this,
          [this]() { emit moveDownRequested(index); });

  for (QCheckBox *checkBox : {redCheckBox, greenCheckBox, blueCheckBox}) {
    connect(checkBox, &QCheckBox::toggled, this,
            [this, checkBox]() { onChannelToggled(checkBox); });
  }
}

void FilterWidget::onChannelToggled(QCheckBox *checkBox) {
  Channel channels = selectedChannels();
  if (channels == Channel::None) {
    QSignalBlocker blocker(checkBox);
    checkBox->setChecked(true);
    return;
  }
  emit channelsChanged(index, channels);
}

Channel FilterWidget::selectedChannels() const {
  Channel channels = Channel::None;
  if (redCheckBox->isChecked()) {
    channels |= Channel::Red;
  }
  if (greenCheckBox->isChecked()) {
    channels |= Channel::Green;
  }
  if (blueCheckBox->isChecked()) {
    channels |= Channel::Blue;
  }
  return channels;
}
