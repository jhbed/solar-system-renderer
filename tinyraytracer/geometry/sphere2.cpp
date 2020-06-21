#include <geometry/light.h>
#include <geometry/sphere2.h>
#include <geometry/transform.h>
#include <rendering/util.h>

#include <cmath>
#include <iostream>

Sphere2::Sphere2(const Transform transform, const float &r,
                 const Material &material)
    : RayTracerObject(transform), radius(r), material(material){};

IntersectionData Sphere2::rayIntersect(const Vec3f &orig,
                                       const Vec3f &dir) const {
  bool isCollision;
  float interval;

  Vec3f o = invTransMatrix.vecmul(orig);
  Vec3f d = invRotationMatrix.vecmul(dir);

  Vec3f L = o * -1;
  //(0 to 1) how orthoganal the direction of the ray is to the direction of the
  // circle this is the angle between dir and L.
  float tca = L.dot(d);

  float d2 = L.dot(L) - tca * tca;

  // quick check to see if there is a collison, see Jon diagrams
  // below thing is a factory function
  if (d2 > radius * radius) return intersection_data::none();

  float thc = sqrtf(radius * radius - d2);
  // the magnitude of the ray, if you plug this into ray equation,
  // you get the point of intersection o+dir*t0 gets point of intersection
  interval = tca - thc;
  // probably just checking to see if our ray is on negative side of the L
  // vector
  float t1 = tca + thc;

  // checking if collision is happening in oppoisite direction of ray, which is
  // wrong
  if (interval < 0) interval = t1;
  if (interval < 0) return intersection_data::none();

  // o+(dir*interval) returns point in this objects orientation, which doesn't
  // seem right this could cause problems later.

  Vec3f relativePoint = o + (d * interval);
  Vec3f globalPoint = transformMatrix.vecmul(relativePoint);
  float alpha = material.getColor(relativePoint, 1.0).w;
  if (alpha == 0) {
    
    return intersection_data::none();
  }
  float lightPenetration = 1. - alpha;

  return IntersectionData(true, interval, lightPenetration, globalPoint);
};
Vec3f Sphere2::getColor(const std::vector<Light> &lights,
                        const std::vector<const RayTracerObject *> &objects,
                        const Vec3f &collisionPoint, const Vec3f &eyePos,
                        const Vec3f &eyeDir) const {

  Vec3f localPoint = invTransMatrix.vecmul(collisionPoint);
  Vec3f localOrigin(0, 0, 0);
  Vec3f globalNormal = (collisionPoint - transformMatrix.vecmul(localOrigin)).normalize();
  Vec3f vtoC = (eyePos - collisionPoint).normalize();

  float diffuse_light_intensity = 0;
  float specular_light_intensity = 0;
  float rimLightScale;
  for (size_t i = 0; i < lights.size(); i++) {
    Vec3f light_dir = (lights[i].position - collisionPoint).normalize();

    float lightPenetration = 1.0;
    for (const auto &s : objects) {
      IntersectionData data = s->rayIntersect(
          collisionPoint, (lights[i].position - collisionPoint).normalize());
      if (data.isIntersect) {
        lightPenetration -= (1.0f - data.lightPenetration);
        break;
      }
    }

    lightPenetration = std::min(1.0f, lightPenetration);
    if (lightPenetration >= 0) {
      diffuse_light_intensity += lights[i].intensity *
                                 std::max(0.f, light_dir.dot(globalNormal)) *
                                 lightPenetration;
           
    }


    /*
    TODO: This is going to do the wrong thing when we have multiple lights so fix this
    */
    float facingFactor = (eyePos - transform.location).normalize().dot((lights[i].position - transform.location).normalize());
    rimLightScale = globalNormal.dot(light_dir);

    facingFactor = std::min(0.f, facingFactor);
    rimLightScale = powf(std::abs(facingFactor), 4) * powf(1 - std::abs(rimLightScale), 4) * 4;


  }

  float rimLight = 1.0 - vtoC.dot(globalNormal);
  rimLight = rendering::smoothstep(0, 1, rimLight);
  rimLight = powf(rimLight, 4.0);


  return material.getColor(localPoint, diffuse_light_intensity + specular_light_intensity + rimLight * rimLightScale);
};


void Sphere2::preRender() {

  transformMatrix = getTransformMatrix();
  invTransMatrix = transformMatrix.invert();

  rotationMatrix = getRotationMatrix();
  invRotationMatrix = rotationMatrix.invert();

}
