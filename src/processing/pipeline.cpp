#include "pipeline.h"

Pipeline::Pipeline() { filters = std::queue<Filter>(); }

void Pipeline::addFilter(const Filter &filter) { filters.push(filter); }

void Pipeline::apply(cv::Mat &image) {
  std::queue<Filter> tempFilters = filters;
  while (!tempFilters.empty()) {
    Filter filter = tempFilters.front();
    filter.apply(image);
    tempFilters.pop();
  }
}
