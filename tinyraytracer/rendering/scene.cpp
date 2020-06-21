#include "scene.h"

#include <geometry/light.h>
#include <geometry/ray_tracer_object.h>
#include <geometry/ring.h>
#include <geometry/sphere2.h>
#include <geometry/vec3f.h>
#include <rendering/rgba_image.h>
#include <rendering/textured_ring_material.h>
#include <rendering/textured_sphere_material.h>
#include <rendering/util.h>

#include <algorithm>
#include <cmath>
#include <iostream>

#include "data/light_data.h"
#include "data/raytracer_object_data.h"
#include "data/scene_data.h"
#include "geometry/empty.h"
#include "geometry/mat4f.h"
#include "rendering/frame_effects.h"
#include "rendering/intersect_object.h"
#include "rendering/earth_material.h"

namespace {

RGBAImage* getImage(
    std::string imageName,
    std::map<std::string, std::unique_ptr<RGBAImage>>& imageLibrary) {
  if (imageLibrary.find(imageName) != imageLibrary.end()) {
    return imageLibrary.at(imageName).get();
  } else {
    RGBAImage* image_ptr = new RGBAImage(imageName);
    imageLibrary.insert(
        std::make_pair(imageName, std::unique_ptr<RGBAImage>(image_ptr)));
    return image_ptr;
  }
}

RayTracerObject* addObj(
    std::vector<std::unique_ptr<RayTracerObject>>& objects,
    const RaytracerObjectData& data,
    std::map<std::string, std::unique_ptr<RGBAImage>>& imageLibrary,
    std::vector<std::unique_ptr<Material>>& materialLibrary) {
  RayTracerObject* obj = nullptr;
  if (data.getRaytracerObjectType() == RaytracerObjectType::ring) {
    RGBAImage* image_ptr = getImage(data.getImageFileName(), imageLibrary);

    TexturedRingMaterial* mat = new TexturedRingMaterial(
        30.0f, *image_ptr, Vec3f(1., 1., 1.), data.getInnerRadius(),
        data.getRadius());  // originally outer rad of 13
    materialLibrary.push_back(std::unique_ptr<TexturedRingMaterial>(mat));

    obj = new Ring(Transform(data.getCenter(), data.getRotation()),
                   data.getInnerRadius(), data.getRadius(), *mat);

  } else if (data.getRaytracerObjectType() == RaytracerObjectType::sphere) {
    
    RGBAImage* image_ptr = getImage(data.getImageFileName(), imageLibrary);
    Material* mat=nullptr;
    if (data.getImageFileName().find("./assets/earth_light_diffuse.png") != std::string::npos) {
      std::cout << "Creating earth material" << std::endl;
      RGBAImage* nightImagePtr = getImage("./assets/earth_dark_diffuse.png", imageLibrary);
      RGBAImage* specMapPtr = getImage("./assets/earth_specular_map.png", imageLibrary);
      std::cout << "got earth material" << std::endl;
      mat = new EarthMaterial(1.f, *image_ptr, *nightImagePtr, *specMapPtr, Vec3f(1., 1., 1.), data.getCenter());
    } else {
      
      mat = new TexturedSphereMaterial(
        1000.f, *image_ptr, Vec3f(1., 1., 1.), data.getCenter());
    }
    materialLibrary.push_back(std::unique_ptr<Material>(mat));

    obj = new Sphere2(Transform(data.getCenter(), data.getRotation()),
                      data.getRadius(), *mat);
  } else {
    obj = new Empty(Transform(data.getCenter(), data.getRotation()));
  }

  obj->setVelocity(data.getVelocity());
  obj->setAngularVelocity(data.getAngularVelocity());

  objects.push_back(std::unique_ptr<RayTracerObject>(obj));

  for (const RaytracerObjectData& child : data.getChildren()) {
    RayTracerObject* c = addObj(objects, child, imageLibrary, materialLibrary);
    c->setParent(obj);
  }

  return obj;
}
}  // namespace

Scene::Scene(const SceneData& sceneData)
    : width(sceneData.getWidth()),
      height(sceneData.getHeight()),
      fov(sceneData.getFov()),
      milkyWayBackground(new RGBAImage("./assets/milky_way.png")),
      milkyWayBackgroundSphere(new TexturedSphereMaterial(
          1.0f, *milkyWayBackground.get(), Vec3f(1., 1., 1.), Vec3f(0, 0, 0))),
      eye(sceneData.getCameraLocation(), sceneData.getCameraRotation(),
          // Vec3f(12.0f, -1.0f, -25.0f), Vec3f(1.1f, -0.5, 0)
          Vec3f(-7.5, 0, 0), Vec3f(-0.6, -0.6, -0.6)) {
  for (auto& data : sceneData.getObjects()) {
    addObj(objects, data, imageLibrary2, materialLibrary2);
  }

  for (auto& data : sceneData.getLights()) {
    lights.push_back(Light(data.getLocation(), data.getIntensity()));
  }
}

std::vector<unsigned char> Scene::render() const {
  // frameBuffer is a list of RGB colors
  std::vector<Vec3f> frameBuffer(width * height);
  std::vector<float> depthBuffer(width * height);

  for (const auto& object : objects) {
    object->preRender();
  }

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      depthBuffer[i + j * width] = 100000;
    }
  }

  double tanfov2 = tan(fov / 2.);
  Mat4f eyeRot = matrices::createRotationMatrix(
      eye.getDirection().x, eye.getDirection().y, eye.getDirection().z);

  for (size_t j = 0; j < height; j++) {
    for (size_t i = 0; i < width; i++) {
      /*
      We want a value between -1 and 1, so we *2 and -1
      */
      float normalizedX = (2 * (i + 0.5) / (float)width - 1);
      float x = (normalizedX * tanfov2 * width / (float)height);
      float y = (2 * (j + 0.5) / (float)height - 1) * tanfov2;
      /*

      Looks like dir is hardcoded to be exactly backwards from the pixel

      */
      // where we are looking from orig=vec(0,0,0), where the screen is 1 behind
      Vec3f dir = Vec3f(x, -1, y).normalize();
      dir = eyeRot.vecmul(dir).normalize();

      // dir = matrices::createRotationMatrix(0, 0, 0).vecmul(dir);

      std::vector<IntersectObject> intersectObjects;
      for (const auto& object : objects) {
        IntersectionData intersectData =
            object->rayIntersect(eye.getPosition(), dir);
        if (intersectData.isIntersect) {
          intersectObjects.push_back(IntersectObject(
              intersectData, object.get(),
              intersectData.collisionPoint.distance(eye.getPosition())));
        }
      }

      if (intersectObjects.size() != 0) {
        std::sort(intersectObjects.begin(), intersectObjects.end());
        // closestObject is the closest object to camera at this pixel
        IntersectObject closestObject = intersectObjects.at(0);

        if (closestObject.distance < depthBuffer[i + j * width]) {
          depthBuffer[i + j * width] = closestObject.distance;
        }

        std::vector<const RayTracerObject*> otherObjects;
        for (const auto& otherObject : objects) {
          if (otherObject.get() != closestObject.object) {
            otherObjects.push_back(otherObject.get());
          }
        }
        Vec3f color = closestObject.object->getColor(
            lights, otherObjects, closestObject.i.collisionPoint,
            eye.getPosition(), dir);

        int idx = 1;
        while (idx < intersectObjects.size() && color.w < 1.0f) {
          closestObject = intersectObjects.at(idx);
          std::vector<const RayTracerObject*> otherObjects;
          for (const auto& otherObject : objects) {
            if (otherObject.get() != closestObject.object) {
              otherObjects.push_back(otherObject.get());
            }
          }
          Vec3f nextColor = closestObject.object->getColor(
              lights, otherObjects, closestObject.i.collisionPoint,
              eye.getPosition(), dir);

          float a = (1.0f - color.w) * nextColor.w + color.w;
          float r = ((1.0f - color.w) * nextColor.w * nextColor.x +
                     color.w * color.x) /
                    a;
          float g = ((1.0f - color.w) * nextColor.w * nextColor.y +
                     color.w * color.y) /
                    a;
          float b = ((1.0f - color.w) * nextColor.w * nextColor.z +
                     color.w * color.z) /
                    a;

          color = Vec3f(r, g, b, a);
          idx++;
        }

        if (color.w < 1.0f) {
          // Vec3f backgroundColor = milkyWayBackground->getPixelNormalized(
          //     i / (float)width, j / (float)height);
          Vec3f backgroundColor = milkyWayBackgroundSphere->getColor(dir, 1.0);
          float a = (1.0f - color.w) * backgroundColor.w + color.w;
          float r = ((1.0f - color.w) * backgroundColor.w * backgroundColor.x +
                     color.w * color.x) /
                    a;
          float g = ((1.0f - color.w) * backgroundColor.w * backgroundColor.y +
                     color.w * color.y) /
                    a;
          float b = ((1.0f - color.w) * backgroundColor.w * backgroundColor.z +
                     color.w * color.z) /
                    a;
          color = Vec3f(r * a, g * a, b * a, 1.0f);
        }

        // removing background that is not a skybox
        frameBuffer[i + j * width] = color.clamp(0, 1);
    
      } else {
        Vec3f pixelColor = milkyWayBackgroundSphere->getColor(dir, 1.0);
        frameBuffer[i + j * width] = pixelColor;
      }
    }
  }

  std::vector<unsigned char> img_data;
  for (const auto& pixel : frameBuffer) {
    img_data.push_back((unsigned char)(pixel.x * 255));
    img_data.push_back((unsigned char)(pixel.y * 255));
    img_data.push_back((unsigned char)(pixel.z * 255));
    img_data.push_back((unsigned char)255);
  }

  return img_data;
}
void Scene::update(float seconds) {
  // update sphere states
  eye.update(seconds);
  for (auto& object : objects) {
    object->update(seconds);
  }
}
