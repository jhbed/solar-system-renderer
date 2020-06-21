#include "textured_ring_material.h"
#include "math.h"
#include "rgba_image.h"

const float PI = 3.1415926f;
const float twoPi = PI * 2;

TexturedRingMaterial::TexturedRingMaterial(const float specularity,
                                           const RGBAImage &texture,
                                           const Vec3f &albedo,
                                           float innerRadius, float outerRadius)
    : specularity(specularity),
      texture(texture),
      albedo(albedo),
      innerRadius(innerRadius),
      outerRadius(outerRadius) {}

float TexturedRingMaterial::getSpecularity() const { return specularity; }
Vec3f TexturedRingMaterial::getColor(const Vec3f &localPoint, float diffuseLightIntensity) const {
  float dist = sqrt(localPoint.x * localPoint.x + localPoint.z * localPoint.z);
  float percentAccross = (dist - innerRadius) / (outerRadius - innerRadius);
  return texture.getPixelNormalized(percentAccross, 0.5);
}
Vec3f TexturedRingMaterial::getColorScaled(const Vec3f &localPoint, int scaleFactor) const {
  float dist = sqrt(localPoint.x * localPoint.x + localPoint.z * localPoint.z);
  float percentAccross = (dist - innerRadius) / (outerRadius - innerRadius);
  //return texture.getPixelNormalized(percentAccross, 0.5);
  return texture.getPixelAvgNormalized1D(percentAccross, scaleFactor);
}

Vec3f TexturedRingMaterial::getAlbedo() const { return albedo; }