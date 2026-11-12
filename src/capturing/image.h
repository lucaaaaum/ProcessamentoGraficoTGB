#include "provider.h"
#include <QImage>

class Image : public Provider {
private:
  QImage image;

public:
  explicit Image(const QImage &img) : image(img) {}
  void start() override;
  void stop() override;
};
