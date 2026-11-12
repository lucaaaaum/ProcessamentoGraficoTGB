#pragma once

#include "app/controller.h"
#include "capturing/camera.h"
#include "ui/canvas.h"
#include <QMainWindow>
#include <qpushbutton.h>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  void setupMenu();
  void setupLayout();
  void setupConnections();
  void toggleCamera();
  void importImage();

  bool cameraOn = false;

  Camera *camera;
  Controller *controller;
  Canvas *canvas;
  QPushButton *takePictureButton;
  QPushButton *toggleCameraButton;
  QAction *importAction;
};
