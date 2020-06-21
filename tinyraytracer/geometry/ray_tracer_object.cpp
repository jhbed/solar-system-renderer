#include "ray_tracer_object.h"

#include <iostream>

RayTracerObject::RayTracerObject(Transform transform)
    : transform(transform), parent(nullptr) {}
RayTracerObject::~RayTracerObject() {}
void RayTracerObject::setAngularVelocity(Vec3f av) {
  this->angularVelocity = av;
}
void RayTracerObject::setVelocity(Vec3f v) { this->velocity = v; }
void RayTracerObject::update(float seconds) {
  this->transform.rotation =
      this->transform.rotation + angularVelocity * seconds;
  this->transform.location = this->transform.location + velocity * seconds;
}

const RayTracerObject *RayTracerObject::getParent() const { return parent; };
void RayTracerObject::setParent(RayTracerObject *newParent) {
  parent = newParent;
}

Mat4f RayTracerObject::getTransformMatrix() const {
  if (parent == nullptr) {
    return transform.getMatrix();
  }

  return parent->getTransformMatrix().matmul(transform.getMatrix());
}
Mat4f RayTracerObject::getRotationMatrix() const {
  if (parent == nullptr) {
    return transform.getRotationMatrix();
  }

  return parent->getRotationMatrix().matmul(transform.getRotationMatrix());
}

void RayTracerObject::preRender() {}