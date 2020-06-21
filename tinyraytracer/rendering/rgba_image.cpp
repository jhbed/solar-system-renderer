#include <lodepng/lodepng.h>
#include <rendering/frame_effects.h>
#include <rendering/rgba_image.h>

#include <iostream>

RGBAImage::RGBAImage(std::string pngFileName) {
  unsigned error = lodepng::decode(this->imageData, this->imageWidth,
                                   this->imageHeight, pngFileName);

  if (error) {
    std::cout << "Error: " << error;
  }
}
Vec3f RGBAImage::getPixel(int x, int y) const {
  unsigned idx = (y * this->imageWidth + x) * 4;

  int r = imageData[idx];
  int g = imageData[idx + 1];
  int b = imageData[idx + 2];
  int a = imageData[idx + 3];

  return Vec3f((r) / 255.0f, (g) / 255.0f, (b) / 255.0f, (float)a / 255.0f);
}
Vec3f RGBAImage::getPixelNormalized(float x, float y) const {
  int xPos = (int)(x * this->imageWidth);
  int yPos = (int)(y * this->imageHeight);
  if (xPos < 0) {
    xPos = 0;
  }
  if (yPos < 0) {
    yPos = 0;
  }
  if (xPos >= imageWidth) {
    xPos = imageWidth - 1;
  }
  if (yPos >= imageHeight) {
    yPos = imageHeight - 1;
  }
  return this->getPixel(xPos, yPos);
}

Vec3f RGBAImage::getPixelAvgNormalized(float x, float y, int radius) const {
  int xPos = (int)(x * this->imageWidth);
  int yPos = (int)(y * this->imageHeight);
  if (xPos < 0) {
    xPos = 0;
  }
  if (yPos < 0) {
    yPos = 0;
  }
  if (xPos >= imageWidth) {
    xPos = imageWidth - 1;
  }
  if (yPos >= imageHeight) {
    yPos = imageHeight - 1;
  }

  int count = 0;
  float totalR = 0;
  float totalG = 0;
  float totalB = 0;
  float totalA = 0;

  for (int x = xPos - radius; x <= xPos + radius; x++) {
    if (x >= 0 && x < imageWidth) {
      for (int y = yPos - radius; y <= yPos + radius; y++) {
        if (y >= 0 && y < imageHeight) {
          Vec3f currentPixel = this->getPixel(x, y);
          totalR += currentPixel.x;
          totalG += currentPixel.y;
          totalB += currentPixel.z;
          totalA += currentPixel.w;
          count++;
        }
      }
    }
  }

  return Vec3f(totalR / count, totalG / count, totalB / count, totalA / count);
}

Vec3f RGBAImage::getPixelAvgNormalized1D(float x, int radius) const {
  int xPos = (int)(x * this->imageWidth);
  int yPos = 0;
  if (xPos < 0) {
    xPos = 0;
  }
  if (xPos >= imageWidth) {
    xPos = imageWidth - 1;
  }

  int count = 0;
  float totalR = 0;
  float totalG = 0;
  float totalB = 0;
  float totalA = 0;

  for (int x = xPos - radius; x <= xPos + radius; x++) {
    if (x >= 0 && x < imageWidth) {
      Vec3f currentPixel = this->getPixel(x, yPos);
      totalR += currentPixel.x;
      totalG += currentPixel.y;
      totalB += currentPixel.z;
      totalA += currentPixel.w;
      count++;
    }
  }

  return Vec3f(totalR / count, totalG / count, totalB / count, totalA / count);
}