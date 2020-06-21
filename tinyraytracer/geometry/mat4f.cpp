#include "mat4f.h"

#include <math.h>

#include <iomanip>
#include <sstream>
namespace {

Mat4f rotationX(float radians) {
  float m[16];
  float s = sinf(radians);
  float c = cosf(radians);
  m[0] = 1.f;
  m[1] = 0.f;
  m[2] = 0.f;
  m[3] = 0.f;
  m[4] = 0.f;
  m[5] = c;
  m[6] = -s;
  m[7] = 0.f;
  m[8] = 0.f;
  m[9] = s;
  m[10] = c;
  m[11] = 0.f;
  m[12] = 0.f;
  m[13] = 0.f;
  m[14] = 0.f;
  m[15] = 1.f;

  return Mat4f(m);
}

Mat4f rotationY(float radians) {
  float m[16];
  float s = sinf(radians);
  float c = cosf(radians);
  m[0] = c;
  m[1] = 0.f;
  m[2] = s;
  m[3] = 0.f;
  m[4] = 0.f;
  m[5] = 1.f;
  m[6] = 0.f;
  m[7] = 0.f;
  m[8] = -s;
  m[9] = 0.f;
  m[10] = c;
  m[11] = 0.f;
  m[12] = 0.f;
  m[13] = 0.f;
  m[14] = 0.f;
  m[15] = 1.f;

  return Mat4f(m);
}

Mat4f rotationZ(float radians) {
  float m[16];
  float s = sinf(radians);
  float c = cosf(radians);
  m[0] = c;
  m[1] = -s;
  m[2] = 0;
  m[3] = 0;
  m[4] = s;
  m[5] = c;
  m[6] = 0;
  m[7] = 0;
  m[8] = 0;
  m[9] = 0;
  m[10] = 1;
  m[11] = 0;
  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
  return Mat4f(m);
}

}  // namespace

Mat4f::Mat4f() {
  m[0] = 1.f;
  m[1] = 0.f;
  m[2] = 0.f;
  m[3] = 0.f;
  m[4] = 0.f;
  m[5] = 1.f;
  m[6] = 0.f;
  m[7] = 0.f;
  m[8] = 0.f;
  m[9] = 0.f;
  m[10] = 1.f;
  m[11] = 0.f;
  m[12] = 0.f;
  m[13] = 0.f;
  m[14] = 0.f;
  m[15] = 1.f;
}

std::string Mat4f::toString() const {
  std::ostringstream oss;
  oss << std::setprecision(3);
  oss << "[" << m[0] << " " << m[1] << " " << m[2] << " " << m[3] << "]"
      << std::endl;
  oss << "[" << m[4] << " " << m[5] << " " << m[6] << " " << m[7] << "]"
      << std::endl;
  oss << "[" << m[8] << " " << m[9] << " " << m[10] << " " << m[11] << "]"
      << std::endl;
  oss << "[" << m[12] << " " << m[13] << " " << m[14] << " " << m[15] << "]"
      << std::endl;

  return oss.str();
}

Mat4f::Mat4f(float mat[16]) {
  for (int i = 0; i < 16; i++) {
    m[i] = mat[i];
  }
}

Mat4f Mat4f::matmul(const Mat4f& otherMat) const {
  float newMat[16];
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      float val = 0;
      for (int i = 0; i < 4; i++) {
        val += otherMat.m[i * 4 + y] * m[x * 4 + i];

        /*This may be column major*/
        // val += m[i * 4 + y] * otherMat.m[x * 4 + i];
      }
      newMat[x * 4 + y] = val;
    }
  }
  return Mat4f(newMat);
}

Vec3f Mat4f::vecmul(const Vec3f& v) const {
  return Vec3f(m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
               m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
               m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
               m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w);
}

Mat4f matrices::createRotationMatrix(float x, float y, float z) {
  return rotationY(y).matmul(rotationZ(z).matmul(rotationX(x)));
}

Mat4f matrices::createTranslateMatrix(float x, float y, float z) {
  float m[16] = {1., 0, 0, x, 0, 1., 0, y, 0, 0, 1., z, 0, 0, 0, 1.};
  return Mat4f(m);
}

Mat4f Mat4f::invert() {
  float inv[16];

  inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
           m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

  inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
           m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

  inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
           m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

  inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

  inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
           m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

  inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
           m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

  inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
           m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

  inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

  inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
           m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

  inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
           m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

  inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

  inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

  inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
           m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

  inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
           m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

  inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

  float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

  det = 1.0 / det;

  for (int i = 0; i < 16; i++) {
    inv[i] = inv[i] * det;
  }
  return Mat4f(inv);
}
