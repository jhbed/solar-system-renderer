#pragma once
#include "geometry/mat4f.h"
#include "geometry/vec3f.h"

struct Transform {
  Vec3f location;
  Vec3f rotation;

  Transform(Vec3f location, Vec3f rotation);
  Mat4f getMatrix() const;
  Mat4f getRotationMatrix() const;
};