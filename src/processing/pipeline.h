#pragma once

#include "filter.h"

class Pipeline {
private:
  std::vector<Filter> filters;

public:
  Pipeline();
  void addFilter(const Filter &filter);
  void removeFilter(int index);
  void clear();
  void moveFilter(int fromIndex, int toIndex);
  void apply(cv::Mat &image);
  std::vector<Filter> listFilters() const;
};
