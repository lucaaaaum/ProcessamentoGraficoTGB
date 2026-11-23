#include "filter.h"
#include <opencv2/core.hpp>

void Filter::apply(cv::Mat &image) {
  if (channels == Channel::All) {
    filterFunction(image);
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
    filterFunction(single);
    cv::extractChannel(single, planes[i], i);
  }

  cv::merge(planes, image);
}
