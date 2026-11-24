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
  lastOutput.release();
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

void Controller::clearFilters() {
  pipeline->clear();
  emit pipelineChanged(pipeline->listFilters());
  render();
}

void Controller::moveFilter(int fromIndex, int toIndex) {
  pipeline->moveFilter(fromIndex, toIndex);
  emit pipelineChanged(pipeline->listFilters());
  render();
}

void Controller::setFilterChannels(int index, Channel channels) {
  pipeline->setFilterChannels(index, channels);
  render();
}

void Controller::setDisplayChannels(Channel channels) {
  displayChannels = channels;
  render();
}

void Controller::render() {
  if (lastFrame.empty())
    return;
  auto outputFrame = lastFrame.clone();
  pipeline->apply(outputFrame);
  isolateDisplayChannels(outputFrame);
  lastOutput = outputFrame;
  emit frameChanged(outputFrame);
}

cv::Mat Controller::currentOutput() const { return lastOutput.clone(); }

void Controller::isolateDisplayChannels(cv::Mat &frame) const {
  if (displayChannels == Channel::All) {
    return;
  }
  std::vector<cv::Mat> planes;
  cv::split(frame, planes);
  for (int i = 0; i < static_cast<int>(planes.size()); ++i) {
    if (!hasFlag(displayChannels, channelAt(i))) {
      planes[i].setTo(0);
    }
  }
  cv::merge(planes, frame);
}

std::vector<Filter> Controller::listPipelineFilters() const {
  return pipeline->listFilters();
}
