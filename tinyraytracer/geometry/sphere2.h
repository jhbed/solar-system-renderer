#include <geometry/ray_tracer_object.h>
#include <geometry/transform.h>
#include <geometry/vec3f.h>
#include <geometry/mat4f.h>
#include <rendering/material.h>

class Sphere2 : public RayTracerObject {
  float radius;
  const Material &material;

  Mat4f transformMatrix;
  Mat4f invTransMatrix;

  Mat4f rotationMatrix;
  Mat4f invRotationMatrix;

 public:
  // the = 0 means that this is "Pure" - meaning it cannot be implemented in
  // this class must be implemented in subclass
  Sphere2(const Transform transform, const float &r, const Material &material);
  virtual ~Sphere2() {}
  virtual IntersectionData rayIntersect(const Vec3f &orig,
                                        const Vec3f &dir) const override;
  virtual Vec3f getColor(const std::vector<Light> &lights,
                         const std::vector<const RayTracerObject *> &objects,
                         const Vec3f &collisionPoint, const Vec3f &eyePos,
                         const Vec3f &eyeDir) const override;

  virtual void preRender() override;

};