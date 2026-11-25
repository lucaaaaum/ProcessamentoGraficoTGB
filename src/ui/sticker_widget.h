#pragma once

#include <QListWidget>
#include <QSpinBox>
#include <QWidget>
#include <opencv2/core/mat.hpp>
#include <vector>

class StickerWidget : public QWidget {
  Q_OBJECT

private:
  QListWidget *list;
  QSpinBox *sizeBox;
  std::vector<cv::Mat> stickers;
  void loadStickers();

public:
  StickerWidget(QWidget *parent = nullptr);
  cv::Mat selectedSticker() const;
};
