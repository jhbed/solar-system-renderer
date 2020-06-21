#pragma once
#include "geometry/intersection_data.h"
#include "geometry/ray_tracer_object.h"
#include "geometry/transform.h"

class Empty : public RayTracerObject {
 public:
  Empty(Transform transform);
  // what is this?
  virtual ~Empty() override;
  virtual IntersectionData rayIntersect(const Vec3f &orig,
                                        const Vec3f &dir) const override;
  virtual Vec3f getColor(const std::vector<Light> &lights,
                         const std::vector<const RayTracerObject *> &objects,
                         const Vec3f &collisionPoint, const Vec3f &eyePos,
                         const Vec3f &eyeDir) const override;
};