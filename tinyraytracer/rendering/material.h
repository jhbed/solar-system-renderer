#pragma once
#include <geometry/vec3f.h>

class Material {
 public:
  virtual float getSpecularity() const = 0;
  virtual Vec3f getColor(const Vec3f &collisionPoint, float diffuseLightIntensity) const = 0;
  virtual Vec3f getColorScaled(const Vec3f &collisionPoint, int scaleFactor) const = 0;
  virtual Vec3f getAlbedo() const = 0;
};