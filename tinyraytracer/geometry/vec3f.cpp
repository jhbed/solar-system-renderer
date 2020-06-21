#include "vec3f.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
float clampFloat(float num, float lowerBound, float upperBound) {
  if (num < lowerBound) {
    return lowerBound;
  }
  if (num > upperBound) {
    return upperBound;
  }
  return num;
}
}  // namespace
// just like a class, but everything is public by default

//<vector> requires that its objects have an empty constructor
Vec3f::Vec3f() : x(0), y(0), z(0), w(1.0f) {}

/*
    Initializers: Put the actual pointer to the thing in the struct. Works
   simlar to how Java is designed by default (though java makes a copy of the
   pointer).

    Doing something like:
    Vec3f(float x, float y, float z): x(x), y(y), z(z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Would actually copy the integers into the struct. So now you've got two
   copies of the integers. Actually, you have three, though I don't fully get
   where the third copy happens

    */
Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z), w(1.0f) {}

Vec3f::Vec3f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

// overloading the [] operator! so now when you do objectname[1] it will
// actually call operator[](1) since you put float& it isn't returning the
// float, it is returning the location that holds the float that way you can do
// something like objectname[0] = 10 which is the same as saying (memLocation
// x100) = 10 crazy that you can do that!!
float& Vec3f::operator[](int idx) {
  switch (idx) {
    case 0:
      return x;

    case 1:
      return y;

    case 2:
      return z;

    default:
      throw "Vec3f is only size 3";
  }
}

// overloading minus sign operator
Vec3f Vec3f::operator-(const Vec3f& rightSide) const {
  return Vec3f(x - rightSide.x, y - rightSide.y, z - rightSide.z);
}

// overloading minus sign operator
Vec3f Vec3f::operator+(const Vec3f& rightSide) const {
  return Vec3f(x + rightSide.x, y + rightSide.y, z + rightSide.z);
}

// overloading minus sign operator
Vec3f Vec3f::operator*(const float s) const {
  return Vec3f(x * s, y * s, z * s, w * s);
}

Vec3f Vec3f::operator-() const { return Vec3f(-x, -y, -z); }

// making a simple vector vector dot prodcut implementation
float Vec3f::dot(const Vec3f& vec2) const {
  return x * vec2.x + y * vec2.y + z * vec2.z;
}

float Vec3f::distance(const Vec3f& vec2) const {
  float x_dist = x - vec2.x;
  float y_dist = y - vec2.y;
  float z_dist = z - vec2.z;
  return sqrt(x_dist * x_dist + y_dist * y_dist + z_dist * z_dist);
}

// equation for the magnitude of a vector
// this is simply using pythagorean's theorem in 3d to get the hypotenuse!!
float Vec3f::magnitude() const { return sqrt(x * x + y * y + z * z); }

// This makes magnitude=1 (a unit vector?)
Vec3f Vec3f::normalize() const {
  float m = magnitude();
  if (m == 0) {
    return Vec3f(0, 0, 0);
  } else {
    return Vec3f(x / m, y / m, z / m);
  }
}

Vec3f Vec3f::clamp(float lowerBound, float upperBound) const {
  return Vec3f(clampFloat(x, lowerBound, upperBound),
               clampFloat(y, lowerBound, upperBound),
               clampFloat(z, lowerBound, upperBound));
}

std::string Vec3f::toString() const {
  std::ostringstream oss;
  oss << std::setprecision(3);
  oss << "(" << x << "," << y << "," << z << "," << w << ")" << std::endl;

  return oss.str();
}
