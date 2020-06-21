#pragma once
#include "geometry/vec3f.h"

class AnimatedCamera {
  Vec3f position;
  Vec3f direction;
  Vec3f velocity;
  Vec3f angularVelocity;
  float focalDepth;
  float focalWindow;

 public:
  AnimatedCamera(Vec3f position, Vec3f direction, Vec3f velocity,
                 Vec3f angularVelocity);
  void update(float seconds);
  const Vec3f& getPosition() const;
  const Vec3f& getDirection() const;
  float getFocalDepth() const;
  float getFocalWindow() const;
};