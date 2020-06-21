#pragma once
#include <string>
#include "vec3f.h"
class Mat4f {
  float m[16];

 public:
  Mat4f();
  Mat4f(float m[16]);
  Mat4f matmul(const Mat4f& otherMat) const;
  Vec3f vecmul(const Vec3f& vec) const;
  Mat4f invert();
  std::string toString() const;
};

namespace matrices {
Mat4f createRotationMatrix(float x, float y, float z);
Mat4f createTranslateMatrix(float x, float y, float z);

}  // namespace matrices
