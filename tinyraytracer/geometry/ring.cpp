#include "ring.h"

#include <rendering/util.h>

#include <cmath>
#include <iostream>

#include "intersection_data.h"
#include "light.h"
#include "transform.h"
#include "vec3f.h"

Ring::Ring(Transform transform, float innerRadius, float outerRadius,
           Material &material)
    : RayTracerObject(transform),
      innerRadius(innerRadius),
      outerRadius(outerRadius),
      material(material){};

IntersectionData Ring::rayIntersect(const Vec3f &orig, const Vec3f &dir) const {


  // convert global ray to local ray
  Vec3f o = invTransMatrix.vecmul(orig);
  Vec3f d = invRotationMatrix.vecmul(dir);

  // interval
  float t = -o.y / d.y;

  if (t < 0) {
    return intersection_data::none();
  }

  // the point where the ray intersects with y z plane
  Vec3f point = o + (d * t);

  float distanceFromCenter = sqrt(point.x * point.x + point.z * point.z);

  float lightPenetration = 1.0f;
  bool intersects = false;
  if (distanceFromCenter > innerRadius && distanceFromCenter < outerRadius) {
    Vec3f rawColor = material.getColor(point, lightPenetration);
    lightPenetration = 1.0 - rawColor.w;
    intersects = true;
  }
  Vec3f globalP = transformMatrix.vecmul(point);
  return IntersectionData(intersects, t, lightPenetration, globalP);
};

Vec3f Ring::getColor(const std::vector<Light> &lights,
                     const std::vector<const RayTracerObject *> &objects,
                     const Vec3f &collisionPoint, const Vec3f &eyePos,
                     const Vec3f &eyeDir) const {
  // Vec3f normal = transform.rotate(Vec3f(0, 1, 0));

  // global normal up and down
  Vec3f normal = rotationMatrix.vecmul(Vec3f(0, 1, 0));
  Vec3f downNormal = rotationMatrix.vecmul(Vec3f(0, -1, 0));

  float diffuse_light_intensity = 0;
  float specular_light_intensity = 0;

  for (size_t i = 0; i < lights.size(); i++) {
    Vec3f light_dir = (lights[i].position - collisionPoint).normalize();

    bool anyIntersect = false;
    for (const auto &s : objects) {
      IntersectionData data = s->rayIntersect(
          collisionPoint, (lights[i].position - collisionPoint).normalize());
      if (data.isIntersect) {
        anyIntersect = true;
        break;
      }
    }
    if (!anyIntersect) {
      diffuse_light_intensity +=
          lights[i].intensity * std::max(0.f, light_dir.dot(normal));

      // down normals
      diffuse_light_intensity +=
          lights[i].intensity * std::max(0.f, light_dir.dot(downNormal));

    }
  }
  Vec3f rawColor = material.getColor(invTransMatrix.vecmul(collisionPoint), diffuse_light_intensity);
  Vec3f col = rawColor * diffuse_light_intensity;

  col.w = rawColor.w;

  // if we have transparancy, find out what is behind us and get it's
  // color/transparancy run equation in scene on these two colors

  return col;
}

void Ring::preRender() {

  transformMatrix = getTransformMatrix();
  invTransMatrix = transformMatrix.invert();

  rotationMatrix = getRotationMatrix();
  invRotationMatrix = rotationMatrix.invert();

}