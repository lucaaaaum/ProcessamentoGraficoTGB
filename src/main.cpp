#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qwidget.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QWidget window;
  auto *mainLayout = new QVBoxLayout(&window);

  auto *camera = new QLabel("Câmera");
  camera->setAlignment(Qt::AlignCenter);
  camera->setMinimumSize(640, 480);
  mainLayout->addWidget(camera, 1);

  auto *buttons = new QHBoxLayout();
  auto *takePictureButton = new QPushButton("Tirar Foto");
  buttons->addWidget(takePictureButton);
  mainLayout->addLayout(buttons);

  QObject::connect(takePictureButton, &QPushButton::clicked,
                   [] { qDebug() << "Botão 'Tirar Foto' clicado!"; });

  window.show();

  return app.exec();
}
