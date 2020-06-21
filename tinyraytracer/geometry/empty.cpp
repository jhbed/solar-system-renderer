#include "empty.h"

Empty::Empty(Transform transform)
    : RayTracerObject(transform) {}  // what is this?
Empty::~Empty(){};

Vec3f Empty::getColor(const std::vector<Light> &lights,
                      const std::vector<const RayTracerObject *> &objects,
                      const Vec3f &collisionPoint, const Vec3f &eyePos,
                      const Vec3f &eyeDir) const {
  throw "Should never call getColor on Empty";
}

IntersectionData Empty::rayIntersect(const Vec3f &orig,
                                     const Vec3f &dir) const {
  return intersection_data::none();
}