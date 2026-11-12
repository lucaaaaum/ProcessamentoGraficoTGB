#include "ui/canvas.h"
#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStandardPaths>
#include <QTimer>
#include <QVBoxLayout>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow window;
  auto *central = new QWidget;
  auto *mainLayout = new QVBoxLayout(central);
  window.setCentralWidget(central);

  QMenu *fileMenu = window.menuBar()->addMenu("&Arquivo");
  QAction *importAction = fileMenu->addAction("&Importar");

  auto canvas = new Canvas;
  mainLayout->addWidget(canvas, 1);

  canvas->setMessage("Aguardando a câmera...");
  cv::VideoCapture capture(0, cv::CAP_V4L2);
  if (!capture.isOpened()) {
    canvas->setMessage("Erro ao abrir a câmera.");
  }

  QTimer timer;
  QObject::connect(&timer, &QTimer::timeout, [&]() {
    cv::Mat frame;
    if (!capture.read(frame) || frame.empty()) {
      return;
    }
    canvas->setMat(frame);
  });
  timer.start(30);

  QObject::connect(importAction, &QAction::triggered, [&] {
    QString path = QFileDialog::getOpenFileName(
        &window, "Importar Arquivo",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "Imagens (*.png *.jpg *.jpeg *.bmp)", nullptr,
        QFileDialog::DontUseNativeDialog);
    if (path.isEmpty()) {
      qDebug() << "Nenhum arquivo selecionado.";
      return;
    }

    cv::Mat img = cv::imread(path.toStdString());
    if (img.empty()) {
      qDebug() << "Erro ao abrir a imagem.";
      return;
    }
    timer.stop();

    canvas->setMat(img);
  });

  auto *buttons = new QHBoxLayout();
  auto *takePictureButton = new QPushButton("Tirar Foto");
  buttons->addWidget(takePictureButton);
  mainLayout->addLayout(buttons);

  QObject::connect(takePictureButton, &QPushButton::clicked,
                   [] { qDebug() << "Botão 'Tirar Foto' clicado!"; });

  window.show();

  return app.exec();
}
