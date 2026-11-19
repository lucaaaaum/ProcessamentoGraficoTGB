#include "preset_dialog.h"

std::optional<Filter> PresetDialog::getSelectedFilter() const {
  int row = presetList->currentRow();
  if (row < 0 || row >= static_cast<int>(presets.size())) {
    return std::nullopt;
  }
  return presets[row];
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
  presetList->setCurrentRow(0);
  layout->addWidget(presetList);

  auto *buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(buttons);

  connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(presetList, &QListWidget::itemDoubleClicked, this, &QDialog::accept);
}
