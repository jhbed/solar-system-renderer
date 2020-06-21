#pragma once
#include <string>
#include <vector>
#include "geometry/vec3f.h"
#include "raytracer_object_type.h"

class RaytracerObjectData {
  RaytracerObjectType objectType;
  Vec3f center;
  std::string imageFileName;
  float radius;
  Vec3f rotation;
  float innerRadius;
  Vec3f velocity;
  Vec3f angularVelocity;
  std::vector<RaytracerObjectData> children;

 public:
  RaytracerObjectData(RaytracerObjectType object, Vec3f center,
                      std::string imageFileName, float radius, Vec3f rotation,
                      Vec3f velocity, Vec3f angularVelocity,
                      std::vector<RaytracerObjectData> children);

  const RaytracerObjectType& getRaytracerObjectType() const;
  const Vec3f& getCenter() const;
  const std::string& getImageFileName() const;
  const float getRadius() const;
  const Vec3f& getRotation() const;
  const Vec3f& getVelocity() const;
  const Vec3f& getAngularVelocity() const;
  void setInnerRadius(float innerRadius);
  const float getInnerRadius() const;
  const std::vector<RaytracerObjectData>& getChildren() const;
};