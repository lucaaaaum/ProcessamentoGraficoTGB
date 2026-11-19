#include "main_window.h"
#include "capturing/camera.h"
#include "capturing/image.h"
#include "ui/preset_dialog.h"
#include <QDockWidget>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <opencv2/imgcodecs.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  camera = new Camera(0);
  controller = new Controller(this);
  canvas = new Canvas;
  takePictureButton = new QPushButton("Tirar Foto");
  toggleCameraButton = new QPushButton("Ligar");
  setupMenu();
  setupPipelineDock();
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
  connect(controller, &Controller::frameCleared, canvas, &Canvas::clear);
  connect(controller, &Controller::pipelineChanged, pipelineWidget,
          &PipelineWidget::setFilters);
  connect(toggleCameraButton, &QPushButton::clicked, this,
          &MainWindow::toggleCamera);
  connect(importAction, &QAction::triggered, this, &MainWindow::importImage);
  connect(pipelineWidget, &PipelineWidget::addFilterRequested, this, [this]() {
    PresetDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
      return;
    }
    auto filter = dialog.getSelectedFilter();
    if (filter) {
      controller->addFilter(*filter);
    }
  });
  connect(pipelineWidget, &PipelineWidget::removeFilterRequested, controller,
          &Controller::removeFilter);
  connect(pipelineWidget, &PipelineWidget::moveFilterRequested, controller,
          &Controller::moveFilter);
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

void MainWindow::importImage() {
  QString path = QFileDialog::getOpenFileName(
      this, "Importar Arquivo",
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
      "Imagens (*.png *.jpg *.jpeg *.bmp)", nullptr,
      QFileDialog::DontUseNativeDialog);
  if (path.isEmpty()) {
    return;
  }

  auto img = cv::imread(path.toStdString());
  if (img.empty()) {
    canvas->setMessage("Falha ao carregar a imagem.");
    return;
  }

  controller->setProvider(new Image(img));
}

void MainWindow::setupPipelineDock() {
  pipelineDock = new QDockWidget("Pipeline", this);
  pipelineWidget = new PipelineWidget;
  pipelineDock->setWidget(pipelineWidget);
  addDockWidget(Qt::RightDockWidgetArea, pipelineDock);
}
