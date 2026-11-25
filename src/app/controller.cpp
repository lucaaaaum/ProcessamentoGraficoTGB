#include "controller.h"
#include "capturing/image.h"
#include <qtmetamacros.h>

Controller::Controller(QObject *parent)
    : QObject(parent), provider(nullptr), pipeline(new Pipeline) {}

void Controller::setProvider(Provider *prov) {
  if (provider) {
    provider->stop();
    disconnect(connection);
  }
  stickers.clear();
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
  stickers.clear();
  lastFrame.release();
  lastOutput.release();
  emit frameCleared();
}

void Controller::freezeFrame() {
  if (lastFrame.empty()) {
    return;
  }
  setProvider(new Image(lastFrame.clone()));
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

void Controller::reset() {
  pipeline->clear();
  stickers.clear();
  emit pipelineChanged(pipeline->listFilters());
  render();
}

void Controller::addSticker(const cv::Mat &sticker, cv::Point center) {
  if (lastFrame.empty()) {
    return;
  }
  stickers.push_back({sticker, center});
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
  for (const auto &placed : stickers) {
    overlaySticker(outputFrame, placed.image, placed.center);
  }
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
