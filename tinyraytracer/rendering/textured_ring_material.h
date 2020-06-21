#pragma once
#include <geometry/vec3f.h>

#include "material.h"

class RGBAImage;

class TexturedRingMaterial : public Material {
  // shinyness
  float specularity;
  const RGBAImage &texture;
  // how much ambient light gets reflected, how much other stuff gets reflected
  Vec3f albedo;
  float innerRadius;
  float outerRadius;

 public:
  TexturedRingMaterial(const float specularity, const RGBAImage &texture,
                       const Vec3f &albedo, float innerRadius,
                       float outerRadius);
  virtual float getSpecularity() const override;
  virtual Vec3f getColor(const Vec3f &localPoint, float diffuseLightIntensity) const override;
  virtual Vec3f getAlbedo() const override;
  virtual Vec3f getColorScaled(const Vec3f &localPoint, int scaleFactor) const override;
};