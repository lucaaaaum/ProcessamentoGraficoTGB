#pragma once

#include "filter.h"
#include <queue>

class Pipeline {
private:
  std::queue<Filter> filters;

public:
  void addFilter(const Filter &filter);
  void apply(cv::Mat &image);
};
