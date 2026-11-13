#include "pipeline.h"

Pipeline::Pipeline() { filters = std::vector<Filter>(); }

void Pipeline::addFilter(const Filter &filter) { filters.push_back(filter); }

void Pipeline::removeFilter(int index) {
  if (index >= 0 && index < filters.size()) {
    filters.erase(filters.begin() + index);
  }
}

void Pipeline::clear() { filters.clear(); }

void Pipeline::moveFilter(int fromIndex, int toIndex) {
  if (fromIndex >= 0 && fromIndex < filters.size() && toIndex >= 0 &&
      toIndex < filters.size()) {
    Filter filter = filters[fromIndex];
    filters.erase(filters.begin() + fromIndex);
    filters.insert(filters.begin() + toIndex, filter);
  }
}

void Pipeline::apply(cv::Mat &image) {
  for (auto &filter : filters) {
    filter.apply(image);
  }
}

std::vector<Filter> Pipeline::listFilters() const { return filters; }
