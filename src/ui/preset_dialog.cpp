#include "preset_dialog.h"

std::optional<Filter> PresetDialog::getSelectedFilter() const {
  int row = presetList->currentRow();
  if (row < 0 || row >= static_cast<int>(presets.size())) {
    return std::nullopt;
  }
  return presets[row];
}

void PresetDialog::updateDescription(int row) {
  if (row < 0 || row >= static_cast<int>(presets.size())) {
    descriptionLabel->clear();
    return;
  }
  descriptionLabel->setText(
      QString::fromStdString(presets[row].getDescription()));
}

PresetDialog::PresetDialog(QWidget *parent)
    : QDialog(parent), presets(Filter::presets()) {
  auto *layout = new QVBoxLayout(this);

  presetList = new QListWidget;
  presetList->setViewMode(QListView::IconMode);
  presetList->setMovement(QListView::Static);
  presetList->setResizeMode(QListView::Adjust);
  presetList->setGridSize(QSize(120, 80));
  for (const auto &filter : presets) {
    presetList->addItem(QString::fromStdString(filter.getName()));
  }
  layout->addWidget(presetList);

  descriptionLabel = new QLabel;
  descriptionLabel->setWordWrap(true);
  descriptionLabel->setMinimumHeight(60);
  descriptionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  layout->addWidget(descriptionLabel);

  auto *buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(buttons);

  connect(presetList, &QListWidget::currentRowChanged, this,
          &PresetDialog::updateDescription);
  connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(presetList, &QListWidget::itemDoubleClicked, this, &QDialog::accept);

  presetList->setCurrentRow(0);
}
