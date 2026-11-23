#include "filter.h"
#include <opencv2/core.hpp>

void Filter::run(cv::Mat &image) {
  if (!operation) {
    filterFunction(image);
    return;
  }
  if (operand.empty()) {
    return;
  }
  cv::Mat resized = operand;
  if (operand.size() != image.size()) {
    cv::resize(operand, resized, image.size());
  }
  operation(image, resized);
}

void Filter::apply(cv::Mat &image) {
  if (channels == Channel::All) {
    run(image);
    return;
  }

  std::vector<cv::Mat> planes;
  cv::split(image, planes);

  for (int i = 0; i < static_cast<int>(planes.size()); ++i) {
    if (!hasFlag(channels, channelAt(i))) {
      planes[i] = cv::Mat::zeros(planes[i].size(), planes[i].type());
      continue;
    }
    cv::Mat single;
    cv::merge(std::vector<cv::Mat>{planes[i], planes[i], planes[i]}, single);
    run(single);
    cv::extractChannel(single, planes[i], i);
  }

  cv::merge(planes, image);
}
