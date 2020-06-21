#pragma once
#include <string>

struct Vec3f {
  float x;
  float y;
  float z;
  float w;

  Vec3f();
  Vec3f(float x, float y, float z);
  Vec3f(float x, float y, float z, float w);
  float& operator[](int idx);

  // overloading minus sign operator
  Vec3f operator-(const Vec3f& rightSide) const;

  // overloading minus sign operator
  Vec3f operator+(const Vec3f& rightSide) const;

  // overloading minus sign operator
  Vec3f operator*(const float s) const;

  Vec3f operator-() const;

  // making a simple vector vector dot prodcut implementation
  float dot(const Vec3f& vec2) const;

  float distance(const Vec3f& vec2) const;

  // equation for the magnitude of a vector
  // this is simply using pythagorean's theorem in 3d to get the hypotenuse!!
  float magnitude() const;

  // This makes magnitude=1 (a unit vector?)
  Vec3f normalize() const;

  Vec3f clamp(float lowerBound, float upperBound) const;

  std::string toString() const;
};
