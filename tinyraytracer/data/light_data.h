#pragma once
#include <string>
#include <vector>
#include "geometry/vec3f.h"
#include "raytracer_object_type.h"

class LightData {

  Vec3f location;
  float intensity;

 public:
    LightData(Vec3f location, float intensity);
    const Vec3f& getLocation() const;
    float getIntensity() const; 
};