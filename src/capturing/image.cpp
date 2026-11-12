#include "image.h"

void Image::start() {
  emit frameChanged(cv::Mat(image.height(), image.width(), CV_8UC4,
                            const_cast<uchar *>(image.bits()),
                            image.bytesPerLine()));
}

void Image::stop() {
  // No action needed for static images
}
