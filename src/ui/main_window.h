#pragma once

#include "app/controller.h"
#include "capturing/camera.h"
#include "ui/canvas.h"
#include "ui/pipeline_widget.h"
#include <QComboBox>
#include <QMainWindow>
#include <qpushbutton.h>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  void setupMenu();
  void setupLayout();
  void setupPipelineDock();
  void setupConnections();
  void toggleCamera();
  void importImage();
  void addFilterFromPreset();
  void saveImage();
  QString chooseImageFile(const QString &title);

  bool cameraOn = false;

  Camera *camera;
  Controller *controller;
  Canvas *canvas;
  QPushButton *takePictureButton;
  QPushButton *toggleCameraButton;
  QPushButton *saveButton;
  QComboBox *displayChannelBox;
  QAction *importAction;
  QDockWidget *pipelineDock;
  PipelineWidget *pipelineWidget;
};
