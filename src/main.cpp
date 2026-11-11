#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qobjectdefs.h>
#include <qwidget.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QWidget window;
  auto *mainLayout = new QVBoxLayout(&window);

  auto *camera = new QLabel("Abrindo câmera...");
  camera->setAlignment(Qt::AlignCenter);
  camera->setMinimumSize(640, 480);
  mainLayout->addWidget(camera, 1);

  cv::VideoCapture capture(0, cv::CAP_V4L2);
  if (!capture.isOpened()) {
    camera->setText("Erro ao abrir a câmera.");
  }

  QTimer timer;
  QObject::connect(&timer, &QTimer::timeout, [&]() {
    cv::Mat frame;
    if (!capture.read(frame) || frame.empty()) {
      return;
    }

    cv::Mat rgb;
    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
    QImage image(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step),
                 QImage::Format_RGB888);
    camera->setPixmap(QPixmap::fromImage(image).scaled(
        camera->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  });
  timer.start(30);

  auto *buttons = new QHBoxLayout();
  auto *takePictureButton = new QPushButton("Tirar Foto");
  buttons->addWidget(takePictureButton);
  mainLayout->addLayout(buttons);

  QObject::connect(takePictureButton, &QPushButton::clicked,
                   [] { qDebug() << "Botão 'Tirar Foto' clicado!"; });

  window.show();

  return app.exec();
}
