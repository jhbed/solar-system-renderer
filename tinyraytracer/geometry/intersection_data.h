#pragma once
#include <geometry/vec3f.h>

struct IntersectionData {
  bool isIntersect;
  float t0;
  float lightPenetration;
  Vec3f collisionPoint;
  IntersectionData(bool isIntersect, float t0, float lightPenetration,
                   const Vec3f& collisionPoint);
};

namespace intersection_data {
IntersectionData none();
}