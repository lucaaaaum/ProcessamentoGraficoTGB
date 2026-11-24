#include "main_window.h"
#include "capturing/camera.h"
#include "capturing/image.h"
#include "ui/preset_dialog.h"
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <opencv2/imgcodecs.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  camera = new Camera(0);
  controller = new Controller(this);
  canvas = new Canvas;
  takePictureButton = new QPushButton("Tirar Foto");
  toggleCameraButton = new QPushButton("Ligar");
  saveButton = new QPushButton("Salvar");
  displayChannelBox = new QComboBox;
  displayChannelBox->addItem("Todos", static_cast<int>(Channel::All));
  displayChannelBox->addItem("R", static_cast<int>(Channel::Red));
  displayChannelBox->addItem("G", static_cast<int>(Channel::Green));
  displayChannelBox->addItem("B", static_cast<int>(Channel::Blue));
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
  buttons->addWidget(saveButton);
  buttons->addWidget(new QLabel("Exibir canal:"));
  buttons->addWidget(displayChannelBox);
}

void MainWindow::setupConnections() {
  connect(controller, &Controller::frameChanged, canvas, &Canvas::setMat);
  connect(controller, &Controller::frameCleared, canvas, &Canvas::clear);
  connect(controller, &Controller::pipelineChanged, pipelineWidget,
          &PipelineWidget::setFilters);
  connect(toggleCameraButton, &QPushButton::clicked, this,
          &MainWindow::toggleCamera);
  connect(importAction, &QAction::triggered, this, &MainWindow::importImage);
  connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
  connect(pipelineWidget, &PipelineWidget::addFilterRequested, this,
          &MainWindow::addFilterFromPreset);
  connect(pipelineWidget, &PipelineWidget::resetRequested, controller,
          &Controller::clearFilters);
  connect(pipelineWidget, &PipelineWidget::removeFilterRequested, controller,
          &Controller::removeFilter);
  connect(pipelineWidget, &PipelineWidget::moveFilterRequested, controller,
          &Controller::moveFilter);
  connect(pipelineWidget, &PipelineWidget::filterChannelsChangeRequested,
          controller, &Controller::setFilterChannels);
  connect(displayChannelBox, &QComboBox::currentIndexChanged, this, [this]() {
    controller->setDisplayChannels(
        static_cast<Channel>(displayChannelBox->currentData().toInt()));
  });
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

QString MainWindow::chooseImageFile(const QString &title) {
  return QFileDialog::getOpenFileName(
      this, title,
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
      "Imagens (*.png *.jpg *.jpeg *.bmp)", nullptr,
      QFileDialog::DontUseNativeDialog);
}

void MainWindow::addFilterFromPreset() {
  PresetDialog dialog(this);
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }
  auto filter = dialog.getSelectedFilter();
  if (!filter) {
    return;
  }

  if (filter->requiresOperand()) {
    QString path = chooseImageFile("Escolher Segunda Imagem");
    if (path.isEmpty()) {
      return;
    }
    auto operand = cv::imread(path.toStdString());
    if (operand.empty()) {
      QMessageBox::warning(this, "Erro", "Falha ao carregar a imagem.");
      return;
    }
    filter->setOperand(operand, QFileInfo(path).fileName().toStdString());
  }

  controller->addFilter(*filter);
}

void MainWindow::saveImage() {
  cv::Mat output = controller->currentOutput();
  if (output.empty()) {
    QMessageBox::warning(this, "Erro", "Não há imagem para salvar.");
    return;
  }

  QString defaultPath =
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) +
      "/imagem.png";
  QString path = QFileDialog::getSaveFileName(
      this, "Salvar Imagem", defaultPath,
      "PNG (*.png);;JPEG (*.jpg *.jpeg)", nullptr,
      QFileDialog::DontUseNativeDialog);
  if (path.isEmpty()) {
    return;
  }
  if (QFileInfo(path).suffix().isEmpty()) {
    path += ".png";
  }

  bool saved = false;
  try {
    saved = cv::imwrite(path.toStdString(), output);
  } catch (const cv::Exception &) {
    saved = false;
  }
  if (!saved) {
    QMessageBox::warning(this, "Erro", "Falha ao salvar a imagem.");
  }
}

void MainWindow::importImage() {
  QString path = chooseImageFile("Importar Arquivo");
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
