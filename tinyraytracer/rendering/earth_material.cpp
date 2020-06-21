#include "earth_material.h"
#include "math.h"
#include "rgba_image.h"
#include <cmath>
#include <iostream>

const float PI = 3.1415926f;
const float twoPi = PI * 2;

EarthMaterial::EarthMaterial(const float specularity,
                                               const RGBAImage &textureDay,
                                               const RGBAImage &textureNight,
                                               const RGBAImage &specMap,
                                               const Vec3f &albedo,
                                               const Vec3f &sphereCenter)
    : specularity(specularity),
      textureDay(textureDay),
      textureNight(textureNight),
      specMap(specMap),
      albedo(albedo),
      sphereCenter(sphereCenter) {}

float EarthMaterial::getSpecularity() const { return specularity; }
Vec3f EarthMaterial::getColor(const Vec3f &localPoint, float diffuseLightIntensity) const {
  float angle = atan2(localPoint.z, localPoint.x);
  while (angle > twoPi) {
    angle -= twoPi;
  }

  while (angle < 0) {
    angle += twoPi;
  }
  // 2pi is all the way around a circle
  // ange / 2pi is the ratio of the amount around the circle to go
  // 1- that is just reverse the % because it turned out backwards
  float xPercent = 1.0f - (angle / twoPi);

  Vec3f normCollision = localPoint.normalize();

  // add one to get 0-2 scale, divide by 2 to get 0-1 scale, which is the scale
  // we use to map to our picture
  // float yPercent = (1.0f - (normCollision.y + 1.0f) / 2.0f);
  float yPercent;
  yPercent = asin(normCollision.y) / (PI * 0.5f);
  yPercent = (1.0f - (yPercent + 1.0f) / 2.0f);

  //return texture.getPixelNormalized(xPercent, yPercent);
  //changing this back because the start twinkle was too much

  float nightWeight = 1-diffuseLightIntensity;
  float dayWeight = diffuseLightIntensity;
  Vec3f nightColor = textureNight.getPixelNormalized(xPercent, yPercent)*nightWeight;
  Vec3f dayColor = textureDay.getPixelNormalized(xPercent, yPercent)*dayWeight;
  return Vec3f(dayColor.x + nightColor.x, dayColor.y + nightColor.y, dayColor.z + nightColor.z); 
  
  // return texture.getPixelNormalized(0.5, 0.5);
}
Vec3f EarthMaterial::getAlbedo() const { return albedo; }

Vec3f EarthMaterial::getColorScaled(const Vec3f &localPoint, int scaleFactor) const { 
  float angle = atan2(localPoint.z, localPoint.x);
  while (angle > twoPi) {
    angle -= twoPi;
  }

  while (angle < 0) {
    angle += twoPi;
  }
  // 2pi is all the way around a circle
  // ange / 2pi is the ratio of the amount around the circle to go
  // 1- that is just reverse the % because it turned out backwards
  float xPercent = 1.0f - (angle / twoPi);

  Vec3f normCollision = localPoint.normalize();

  // add one to get 0-2 scale, divide by 2 to get 0-1 scale, which is the scale
  // we use to map to our picture
  // float yPercent = (1.0f - (normCollision.y + 1.0f) / 2.0f);
  float yPercent;
  yPercent = asin(normCollision.y) / (PI * 0.5f);
  yPercent = (1.0f - (yPercent + 1.0f) / 2.0f);

  return textureDay.getPixelAvgNormalized(xPercent, yPercent, scaleFactor); 
}