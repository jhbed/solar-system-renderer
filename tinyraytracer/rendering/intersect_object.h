#pragma once
#include "geometry/intersection_data.h"
#include "geometry/ray_tracer_object.h"

struct IntersectObject {
  IntersectionData i;
  RayTracerObject *object;
  float distance;

  IntersectObject(IntersectionData i, RayTracerObject *object, float distance);
  bool operator<(const IntersectObject &other) const;
};