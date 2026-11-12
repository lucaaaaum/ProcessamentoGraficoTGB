#include "controller.h"
#include <qtmetamacros.h>

Controller::Controller(QObject *parent)
    : QObject(parent), provider(nullptr), pipeline(new Pipeline) {}

void Controller::setProvider(Provider *prov) {
  if (provider) {
    provider->stop();
    disconnect(connection);
  }
  provider = prov;
  connection = connect(provider, &Provider::frameChanged, this,
                       [this](const cv::Mat &frame) {
                         lastFrame = frame.clone();
                         render();
                       });
  provider->start();
}

void Controller::addFilter(const Filter &filter) {
  pipeline->addFilter(filter);
  render();
}

void Controller::render() {
  if (lastFrame.empty())
    return;
  auto outputFrame = lastFrame.clone();
  pipeline->apply(outputFrame);
  emit frameChanged(outputFrame);
}

std::vector<Filter> Controller::listPipelineFilters() const {
  return pipeline->listFilters();
}
