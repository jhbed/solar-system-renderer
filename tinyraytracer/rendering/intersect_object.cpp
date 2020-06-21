#include "rendering/intersect_object.h"

IntersectObject::IntersectObject(IntersectionData i, RayTracerObject *object,
                                 float distance)
    : i(i), object(object), distance(distance){};
bool IntersectObject::operator<(const IntersectObject &other) const {
  return distance < other.distance;
}