#include "image.h"

void Image::start() {
  emit frameChanged(image);
}

void Image::stop() {
  // No action needed for static images
}
