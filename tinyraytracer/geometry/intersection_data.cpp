#include <geometry/intersection_data.h>

IntersectionData::IntersectionData(bool isIntersect, 
                                   float t0, 
                                   float lightPenetration, 
                                   const Vec3f& collisionPoint) : isIntersect(isIntersect), 
                                                                  t0(t0), 
                                                                  lightPenetration(lightPenetration),
                                                                  collisionPoint(collisionPoint) {};

namespace intersection_data {
    IntersectionData none(){
        return IntersectionData(false, 999.9f, 0.0f, Vec3f(0.f, 0.f, 0.f));
    }
}