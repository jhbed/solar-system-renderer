#pragma once
#include <geometry/vec3f.h>
#include <string>
#include <vector>

class RGBAImage {
  std::vector<unsigned char> imageData;
  unsigned int imageWidth;
  unsigned int imageHeight;

 public:
  RGBAImage(std::string pngFileName);
  Vec3f getPixel(int x, int y) const;
  Vec3f getPixelNormalized(float x, float y) const;
  Vec3f getPixelAvgNormalized(float x, float y, int boxLength) const;
  Vec3f getPixelAvgNormalized1D(float x, int boxLength) const;
};