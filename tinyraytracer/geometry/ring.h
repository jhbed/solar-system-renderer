#pragma once
#include "ray_tracer_object.h"
#include "rendering/material.h"

struct Transform;

class Ring : public RayTracerObject {
  float innerRadius;
  float outerRadius;
  Material &material;

  Mat4f transformMatrix;
  Mat4f invTransMatrix;

  Mat4f rotationMatrix;
  Mat4f invRotationMatrix;

 public:
  Ring(Transform transform, float innerRadius, float outerRadius,
       Material &material);

  virtual IntersectionData rayIntersect(const Vec3f &orig,
                                const Vec3f &dir) const override;
  virtual Vec3f getColor(const std::vector<Light> &lights,
                 const std::vector<const RayTracerObject *> &objects,
                 const Vec3f &collisionPoint, const Vec3f &eyePos,
                 const Vec3f &eyeDir) const override;

  virtual void preRender() override;
};