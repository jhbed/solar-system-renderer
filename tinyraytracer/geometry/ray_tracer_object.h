#pragma once
#include <geometry/intersection_data.h>
#include <geometry/transform.h>
#include <rendering/material.h>
#include <vector>
#include "vec3f.h"

struct Light;

class RayTracerObject {
 protected:
  Transform transform;
  Vec3f velocity;
  Vec3f angularVelocity;
  RayTracerObject *parent;

 public:
  RayTracerObject(Transform transform);
  // what is this?
  virtual ~RayTracerObject();
  // the = 0 means that this is "Pure" - meaning it cannot be implemented in
  // this class must be implemented in subclass
  virtual IntersectionData rayIntersect(const Vec3f &orig,
                                        const Vec3f &dir) const = 0;
  virtual Vec3f getColor(const std::vector<Light> &lights,
                         const std::vector<const RayTracerObject *> &objects,
                         const Vec3f &collisionPoint, const Vec3f &eyePos,
                         const Vec3f &eyeDir) const = 0;

  virtual void preRender();

  virtual void update(float seconds);
  void setAngularVelocity(Vec3f av);
  void setVelocity(Vec3f v);
  const RayTracerObject *getParent() const;
  void setParent(RayTracerObject *parent);
  Mat4f getTransformMatrix() const;
  Mat4f getRotationMatrix() const;
};