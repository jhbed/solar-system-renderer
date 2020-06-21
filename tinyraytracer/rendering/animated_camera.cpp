#include "rendering/animated_camera.h"

#include <iostream>

AnimatedCamera::AnimatedCamera(Vec3f position, Vec3f direction, Vec3f velocity,
                               Vec3f angularVelocity)
    : position(position),
      direction(direction),
      velocity(velocity),
      angularVelocity(angularVelocity),
      focalDepth(30.0f),
      focalWindow(5.0f) {}
void AnimatedCamera::update(float seconds) {
  position = position + (velocity * seconds);
  direction = direction + (angularVelocity * seconds);
}

const Vec3f& AnimatedCamera::getPosition() const { return position; }
float AnimatedCamera::getFocalDepth() const { return focalDepth; }
float AnimatedCamera::getFocalWindow() const { return focalWindow; }
const Vec3f& AnimatedCamera::getDirection() const { return direction; }
