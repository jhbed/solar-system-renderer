#include "util.h"

#include <math.h>

#include <algorithm>

Vec3f rendering::reflect(Vec3f direction, Vec3f normal) {
  return direction - (normal * 2. * direction.dot(normal));
}

float rendering::clamp(float num, float lowerBound, float upperBound) {
  if (num < lowerBound) {
    return lowerBound;
  }
  if (num > upperBound) {
    return upperBound;
  }
  return num;
}

float rendering::smoothstep(float edge0, float edge1, float x) {
  // Scale, bias and saturate x to 0..1 range
  x = clamp((x - edge0) / (edge1 - edge0), 0.0,
            1.0);  // Evaluate polynomial
  return x * x * (3 - 2 * x);
}

Vec3f rendering::roundVec3f(Vec3f& v, int precision) {
  int factor = 1;
  for (int i = 0; i < precision; i++) {
    factor *= 10;
  }
  float x = roundf(v.x * factor) / factor;
  float y = roundf(v.y * factor) / factor;
  float z = roundf(v.z * factor) / factor;
  float w = roundf(v.w * factor) / factor;
  return Vec3f(x, y, z, w);
}