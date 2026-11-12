#include "canvas.h"
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
  QSize target = image.size().scaled(size(), Qt::KeepAspectRatio);
  QRect area(
      QPoint((width() - target.width()) / 2, (height() - target.height()) / 2),
      target);
  painter.drawImage(area, image);
}
