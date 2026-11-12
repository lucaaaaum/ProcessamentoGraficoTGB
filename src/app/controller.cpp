#include "controller.h"

void Controller::setProvider(Provider *prov) {
  if (provider) {
    provider->stop();
    disconnect(connection);
  }
  provider = prov;
  connection = connect(provider, &Provider::frameChanged, this,
                       [this](const cv::Mat &frame) {
                         cv::Mat outputFrame = frame.clone();
                         if (pipeline) {
                           pipeline->apply(outputFrame);
                         }
                         emit frameChanged(outputFrame);
                       });
  provider->start();
}

void Controller::addFilter(const Filter &filter) {
  pipeline->addFilter(filter);
}
