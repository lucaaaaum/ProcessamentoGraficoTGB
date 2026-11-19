#include "filter_widget.h"
#include <QHBoxLayout>
#include <QLabel>

FilterWidget::FilterWidget(const Filter &filter, int index, QWidget *parent)
    : QWidget(parent), index(index) {
  setupLayout(filter.getName());
  setupConnections();
}

void FilterWidget::setupLayout(const std::string &name) {
  auto *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  moveUpButton = new QPushButton("↑");
  moveDownButton = new QPushButton("↓");
  removeButton = new QPushButton("✕");

  layout->addWidget(new QLabel(QString::fromStdString(name)), 1);
  layout->addWidget(moveUpButton);
  layout->addWidget(moveDownButton);
  layout->addWidget(removeButton);
}

void FilterWidget::setupConnections() {
  connect(removeButton, &QPushButton::clicked, this,
          [this]() { emit removeRequested(index); });
  connect(moveUpButton, &QPushButton::clicked, this,
          [this]() { emit moveUpRequested(index); });
  connect(moveDownButton, &QPushButton::clicked, this,
          [this]() { emit moveDownRequested(index); });
}
