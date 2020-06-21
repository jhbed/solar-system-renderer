#include "textured_sphere_material.h"
#include "math.h"
#include "rgba_image.h"

const float PI = 3.1415926f;
const float twoPi = PI * 2;

TexturedSphereMaterial::TexturedSphereMaterial(const float specularity,
                                               const RGBAImage &texture,
                                               const Vec3f &albedo,
                                               const Vec3f &sphereCenter)
    : specularity(specularity),
      texture(texture),
      albedo(albedo),
      sphereCenter(sphereCenter) {}

float TexturedSphereMaterial::getSpecularity() const { return specularity; }
Vec3f TexturedSphereMaterial::getColor(const Vec3f &localPoint, float diffuseLightIntensity) const {
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
  Vec3f rawColor = texture.getPixelNormalized(xPercent, yPercent);
  Vec3f col = rawColor * diffuseLightIntensity;
  return Vec3f(std::min(col.x, 1.0f), std::min(col.y, 1.0f),
               std::min(col.z, 1.0f), rawColor.w);
  // return texture.getPixelNormalized(0.5, 0.5);
}
Vec3f TexturedSphereMaterial::getAlbedo() const { return albedo; }

Vec3f TexturedSphereMaterial::getColorScaled(const Vec3f &localPoint, int scaleFactor) const { 
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

  return texture.getPixelAvgNormalized(xPercent, yPercent, scaleFactor); 
}