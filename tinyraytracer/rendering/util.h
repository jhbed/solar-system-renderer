#include <geometry/vec3f.h>

namespace rendering {
Vec3f reflect(Vec3f direction, Vec3f normal);
float smoothstep(float edge0, float edge1, float x);
float clamp(float num, float lowerBound, float upperBound);
Vec3f roundVec3f(Vec3f& v, int precision);
}  // namespace rendering