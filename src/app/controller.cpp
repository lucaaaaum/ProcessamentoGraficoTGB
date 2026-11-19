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

void Controller::cleanProvider() {
  if (provider) {
    provider->stop();
    disconnect(connection);
    provider = nullptr;
  }
  lastFrame.release();
  emit frameCleared();
}

void Controller::addFilter(const Filter &filter) {
  pipeline->addFilter(filter);
  emit pipelineChanged(pipeline->listFilters());
  render();
}

void Controller::removeFilter(int index) {
  pipeline->removeFilter(index);
  emit pipelineChanged(pipeline->listFilters());
  render();
}

void Controller::moveFilter(int fromIndex, int toIndex) {
  pipeline->moveFilter(fromIndex, toIndex);
  emit pipelineChanged(pipeline->listFilters());
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
