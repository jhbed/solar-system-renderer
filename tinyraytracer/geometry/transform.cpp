#include "transform.h"

#include "geometry/mat4f.h"

Transform::Transform(Vec3f location, Vec3f rotation)
    : location(location), rotation(rotation) {}

Mat4f Transform::getMatrix() const {
  return matrices::createTranslateMatrix(location.x, location.y, location.z)
      .matmul(getRotationMatrix());
}

Mat4f Transform::getRotationMatrix() const {
  return matrices::createRotationMatrix(rotation.x, rotation.y, rotation.z);
}
