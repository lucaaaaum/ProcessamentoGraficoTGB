#pragma once

#include "filter.h"
#include <queue>

class Pipeline {
private:
  std::queue<Filter> filters;

public:
  Pipeline();
  void addFilter(const Filter &filter);
  void apply(cv::Mat &image);
  std::vector<Filter> listFilters() const;
};
