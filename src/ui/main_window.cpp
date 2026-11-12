#include "main_window.h"
#include "capturing/camera.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  camera = new Camera(0);
  controller = new Controller(this);
  canvas = new Canvas;
  takePictureButton = new QPushButton("Tirar Foto");
  toggleCameraButton = new QPushButton("Ligar");
  setupMenu();
  setupLayout();
  setupConnections();
}

void MainWindow::setupMenu() {
  QMenu *fileMenu = menuBar()->addMenu("&Arquivo");
  importAction = fileMenu->addAction("&Importar");
}

void MainWindow::setupLayout() {
  auto *central = new QWidget;
  setCentralWidget(central);

  auto *mainLayout = new QHBoxLayout(central);
  auto *imageLayout = new QVBoxLayout(central);
  mainLayout->addLayout(imageLayout, 1);
  imageLayout->addWidget(canvas, 1);

  auto *buttons = new QHBoxLayout();
  imageLayout->addLayout(buttons);
  buttons->addWidget(takePictureButton);
  buttons->addWidget(toggleCameraButton);
}

void MainWindow::setupConnections() {
  connect(controller, &Controller::frameChanged, canvas, &Canvas::setMat);
  connect(toggleCameraButton, &QPushButton::clicked, this,
          &MainWindow::toggleCamera);
}

void MainWindow::toggleCamera() {
  if (cameraOn) {
    controller->cleanProvider();
    toggleCameraButton->setText("Ligar");
  } else {
    controller->setProvider(camera);
    toggleCameraButton->setText("Desligar");
  }
  cameraOn = !cameraOn;
}
