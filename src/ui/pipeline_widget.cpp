#include "pipeline_widget.h"
#include "ui/filter_widget.h"
#include <QLabel>

PipelineWidget::PipelineWidget(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  filtersLayout = new QVBoxLayout();
  layout->addLayout(filtersLayout);
  layout->addStretch();
  layout->addWidget(addFilterButton = new QPushButton("Adicionar Filtro"));
  connect(addFilterButton, &QPushButton::clicked, this,
          &PipelineWidget::addFilterRequested);
}
void PipelineWidget::setFilters(const std::vector<Filter> &filters) {
  while (QLayoutItem *item = filtersLayout->takeAt(0)) {
    if (QWidget *widget = item->widget()) {
      widget->deleteLater();
    }
    delete item;
  }
  for (int i = 0; i < static_cast<int>(filters.size()); ++i) {
    auto *filterWidget = new FilterWidget(filters[i], i);
    connect(filterWidget, &FilterWidget::removeRequested, this,
            &PipelineWidget::removeFilterRequested);
    connect(filterWidget, &FilterWidget::moveUpRequested, this,
            [this, i]() { emit moveFilterRequested(i, i - 1); });
    connect(filterWidget, &FilterWidget::moveDownRequested, this,
            [this, i]() { emit moveFilterRequested(i, i + 1); });
    connect(filterWidget, &FilterWidget::channelsChanged, this,
            &PipelineWidget::filterChannelsChangeRequested);
    filtersLayout->addWidget(filterWidget);
  }
}
