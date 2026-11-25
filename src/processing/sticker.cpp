#include "sticker.h"
#include <opencv2/core.hpp>
#include <vector>

void overlaySticker(cv::Mat &frame, const cv::Mat &sticker, cv::Point center) {
  cv::Rect stickerArea(center.x - sticker.cols / 2, center.y - sticker.rows / 2,
                       sticker.cols, sticker.rows);
  cv::Rect visible = stickerArea & cv::Rect(0, 0, frame.cols, frame.rows);
  if (visible.empty()) {
    return;
  }

  cv::Mat target = frame(visible);
  cv::Mat source = sticker(visible - stickerArea.tl());

  std::vector<cv::Mat> channels;
  cv::split(source, channels);

  cv::Mat alpha;
  channels[3].convertTo(alpha, CV_32F, 1.0 / 255.0);
  cv::Mat alpha3;
  cv::merge(std::vector<cv::Mat>{alpha, alpha, alpha}, alpha3);
  cv::Mat inverseAlpha3 = cv::Scalar::all(1.0) - alpha3;

  cv::Mat color;
  cv::merge(std::vector<cv::Mat>{channels[0], channels[1], channels[2]}, color);

  cv::Mat colorF, targetF;
  color.convertTo(colorF, CV_32FC3);
  target.convertTo(targetF, CV_32FC3);

  cv::Mat blended = colorF.mul(alpha3) + targetF.mul(inverseAlpha3);
  blended.convertTo(target, CV_8UC3);
}
