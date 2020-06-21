#include "rendering/frame_effects.h"

#include <chrono>
#include <iostream>

#include "math.h"
#include "rendering/util.h"

namespace {

const float PI = 3.14159265359;

float calcGuassianSampleVal(int x, int y, float sigma) {
  float exponent = (x * x + y * y) / (2 * sigma * sigma);
  float base = 1 / (sqrtf(2 * PI * sigma * sigma));
  return base * exp(-exponent);
}
std::vector<std::vector<float>> generateGaussianMatrix(float sigma,
                                                       int kRadius) {
  std::vector<std::vector<float>> sampleMatrix;
  float total = 0;
  for (int y = -kRadius; y <= kRadius; y++) {
    std::vector<float> row;

    for (int x = -kRadius; x <= kRadius; x++) {
      float val = calcGuassianSampleVal(x, y, sigma);
      total += val;
      row.push_back(val);
    }
    sampleMatrix.push_back(row);
  }

  for (int i = 0; i < sampleMatrix.size(); i++) {
    for (int j = 0; j < sampleMatrix.at(i).size(); j++) {
      sampleMatrix[i][j] = sampleMatrix[i][j] / total;
    }
  }
  return sampleMatrix;
}

Vec3f blurPixelWithMatrix(int i, int j, const std::vector<Vec3f>& img,
                          int width, int height,
                          const std::vector<std::vector<float>>& gaussianMatrix,
                          int kSize) {
  float alpha = img[i + j * width].w;
  Vec3f resultPixel(0, 0, 0);
  for (int y = -kSize; y <= kSize; y++) {
    if (j + y < 0 || j + y >= height) {
      continue;
    }
    for (int x = -kSize; x <= kSize; x++) {
      if (i + x < 0 || i + x >= width) {
        continue;
      }
      Vec3f color = img[i + x + (j + y) * width];
      float sampleContrib = gaussianMatrix[y + kSize][x + kSize];
      resultPixel = resultPixel + color * sampleContrib;
    }
  }
  resultPixel = resultPixel.clamp(0, 1.0f);
  resultPixel.w = alpha;
  return resultPixel;
}

}  // namespace

Vec3f frame_effects::blurPixel(int i, int j, const std::vector<Vec3f>& img,
                               int width, int height, float sigma, int kSize) {
  std::vector<std::vector<float>> gaussianMatrix =
      generateGaussianMatrix(sigma, kSize);
  return blurPixelWithMatrix(i, j, img, width, height, gaussianMatrix, kSize);
}

std::vector<Vec3f> frame_effects::depthOfField(
    const std::vector<float>& depthBuffer, const std::vector<Vec3f>& img,
    int width, int height, float focalDepth, float focusDistance) {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Vec3f> blurredImg;

  float maxBlurriness = 8.0f;
  float depthAtMaxBlurriness = 5.0f;
  int kSize = 5;
  std::vector<std::vector<float>> maxBlurGaussMatrix =
      generateGaussianMatrix(maxBlurriness, kSize);

  float pixelsBlurred = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float depth = depthBuffer[x + y * width];
      float distanceFromFocal = abs(depth - focalDepth);
      if (distanceFromFocal > focusDistance) {
        float blurAmount =
            (distanceFromFocal - focusDistance) / depthAtMaxBlurriness;
        Vec3f pixelVal;
        if (blurAmount >= 1.0f) {
          pixelVal = blurPixelWithMatrix(x, y, img, width, height,
                                         maxBlurGaussMatrix, kSize);
        } else {
          pixelVal = blurPixel(x, y, img, width, height,
                               blurAmount * maxBlurriness, kSize);
          pixelsBlurred += 1;
        }
        blurredImg.push_back(pixelVal);
      } else {
        blurredImg.push_back(img[x + y * width]);
      }
    }
  }
  std::cout << "Calculated Blurred pixels: " << pixelsBlurred << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto interval =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Time to calc Manually blurred pixels: " << interval.count()
            << std::endl;

  return blurredImg;
}