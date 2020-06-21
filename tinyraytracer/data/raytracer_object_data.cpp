#include "raytracer_object_data.h"

RaytracerObjectData::RaytracerObjectData(
    RaytracerObjectType objectType, Vec3f center, std::string imageFileName,
    float radius, Vec3f rotation, Vec3f velocity, Vec3f angularVelocity,
    std::vector<RaytracerObjectData> children)
    : objectType(objectType),
      center(center),
      imageFileName(imageFileName),
      radius(radius),
      rotation(rotation),
      velocity(velocity),
      angularVelocity(angularVelocity),
      children(children) {}

const RaytracerObjectType& RaytracerObjectData::getRaytracerObjectType() const {
  return objectType;
}
const Vec3f& RaytracerObjectData::getCenter() const { return center; }
const std::string& RaytracerObjectData::getImageFileName() const {
  return imageFileName;
}
const float RaytracerObjectData::getRadius() const { return radius; };
const Vec3f& RaytracerObjectData::getRotation() const { return rotation; };
const Vec3f& RaytracerObjectData::getVelocity() const { return velocity; };
const Vec3f& RaytracerObjectData::getAngularVelocity() const {
  return angularVelocity;
};

void RaytracerObjectData::setInnerRadius(float radius) { innerRadius = radius; }
const float RaytracerObjectData::getInnerRadius() const { return innerRadius; }
const std::vector<RaytracerObjectData>& RaytracerObjectData::getChildren()
    const {
  return children;
}