#pragma once

#include <QImage>
#include <QWidget>
#include <opencv2/core/mat.hpp>

class Canvas : public QWidget {
  Q_OBJECT

public:
  explicit Canvas(QWidget *parent = nullptr);
  QSize sizeHint() const override;

public slots:
  void setMat(const cv::Mat &mat);
  void setMessage(const QString &msg);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QImage image;
  QString message;
};
