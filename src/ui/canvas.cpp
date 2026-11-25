#include "canvas.h"
#include <QMouseEvent>
#include <QPainter>
#include <opencv2/imgproc.hpp>

Canvas::Canvas(QWidget *parent) : QWidget(parent) { setMinimumSize(320, 240); }

QSize Canvas::sizeHint() const { return QSize(640, 480); }

void Canvas::setMat(const cv::Mat &mat) {
  image = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step),
                 QImage::Format_BGR888)
              .copy();
  message.clear();
  update();
}

void Canvas::clear() {
  image = QImage();
  message.clear();
  update();
}

void Canvas::setMessage(const QString &msg) {
  message = msg;
  update();
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);

  if (!message.isEmpty()) {
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, message);
    return;
  }

  if (image.isNull()) {
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, "Nenhuma imagem disponível");
    return;
  }

  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.drawImage(imageArea(), image);
}

QRect Canvas::imageArea() const {
  QSize target = image.size().scaled(size(), Qt::KeepAspectRatio);
  return QRect(
      QPoint((width() - target.width()) / 2, (height() - target.height()) / 2),
      target);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton || image.isNull() ||
      !message.isEmpty()) {
    return;
  }
  QRect area = imageArea();
  QPoint pos = event->position().toPoint();
  if (!area.contains(pos)) {
    return;
  }
  int x = (pos.x() - area.x()) * image.width() / area.width();
  int y = (pos.y() - area.y()) * image.height() / area.height();
  emit imageClicked(cv::Point(x, y));
}
