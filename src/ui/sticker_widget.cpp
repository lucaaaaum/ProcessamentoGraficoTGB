#include "sticker_widget.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <algorithm>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

StickerWidget::StickerWidget(QWidget *parent) : QWidget(parent) {
  auto *layout = new QVBoxLayout(this);

  list = new QListWidget;
  list->setViewMode(QListView::IconMode);
  list->setIconSize(QSize(48, 48));
  list->setGridSize(QSize(64, 64));
  list->setResizeMode(QListView::Adjust);
  list->setMovement(QListView::Static);
  layout->addWidget(list);

  auto *sizeLayout = new QHBoxLayout;
  sizeLayout->addWidget(new QLabel("Tamanho:"));
  sizeBox = new QSpinBox;
  sizeBox->setRange(16, 618);
  sizeBox->setValue(128);
  sizeBox->setSuffix(" px");
  sizeLayout->addWidget(sizeBox);
  layout->addLayout(sizeLayout);

  layout->addWidget(new QLabel("Clique na foto para posicionar."));

  loadStickers();
  if (list->count() > 0) {
    list->setCurrentRow(0);
  }
}

void StickerWidget::loadStickers() {
  QDir dir(":/stickers");
  for (const QString &name : dir.entryList(QDir::Files, QDir::Name)) {
    QString path = dir.filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
      continue;
    }
    QByteArray bytes = file.readAll();
    cv::Mat buffer(1, static_cast<int>(bytes.size()), CV_8UC1, bytes.data());
    cv::Mat image = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);
    if (image.empty() || image.channels() != 4) {
      continue;
    }
    stickers.push_back(image);
    auto *item = new QListWidgetItem(QIcon(path), QString());
    item->setToolTip(QFileInfo(name).baseName());
    list->addItem(item);
  }
}

cv::Mat StickerWidget::selectedSticker() const {
  int row = list->currentRow();
  if (row < 0 || row >= static_cast<int>(stickers.size())) {
    return cv::Mat();
  }
  const cv::Mat &original = stickers[row];
  int side = sizeBox->value();
  double scale = static_cast<double>(side) / std::max(original.cols, original.rows);
  cv::Mat resized;
  cv::resize(original, resized, cv::Size(), scale, scale,
             scale < 1.0 ? cv::INTER_AREA : cv::INTER_LINEAR);
  return resized;
}
