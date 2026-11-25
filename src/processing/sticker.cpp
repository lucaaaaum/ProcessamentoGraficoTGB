#include "sticker.h"

void overlaySticker(cv::Mat &frame, const cv::Mat &sticker,
                    cv::Point stickerCenter) {
  int stickerLeft = stickerCenter.x - sticker.cols / 2;
  int stickerTop = stickerCenter.y - sticker.rows / 2;

  for (int stickerRow = 0; stickerRow < sticker.rows; ++stickerRow) {
    for (int stickerColumn = 0; stickerColumn < sticker.cols; ++stickerColumn) {
      int photoRow = stickerTop + stickerRow;
      int photoColumn = stickerLeft + stickerColumn;
      bool isInsideThePhoto = photoRow >= 0 && photoRow < frame.rows &&
                              photoColumn >= 0 && photoColumn < frame.cols;
      if (!isInsideThePhoto) {
        continue;
      }

      cv::Vec4b stickerPixel = sticker.at<cv::Vec4b>(stickerRow, stickerColumn);
      bool isOpaque = stickerPixel[3] > 127;
      if (!isOpaque) {
        continue;
      }

      frame.at<cv::Vec3b>(photoRow, photoColumn) =
          cv::Vec3b(stickerPixel[0], stickerPixel[1], stickerPixel[2]);
    }
  }
}
