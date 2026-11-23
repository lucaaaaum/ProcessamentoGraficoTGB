#pragma once

#include "processing/filter.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <vector>

class PresetDialog : public QDialog {
  Q_OBJECT

private:
  std::vector<Filter> presets;
  QListWidget *presetList;
  QLabel *descriptionLabel;
  void updateDescription(int row);

public:
  explicit PresetDialog(QWidget *parent = nullptr);

  std::optional<Filter> getSelectedFilter() const;
};
