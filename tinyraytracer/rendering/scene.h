#pragma once
#include <geometry/vec3f.h>
#include <rendering/rgba_image.h>
#include <rendering/textured_sphere_material.h>

#include <map>
#include <memory>
#include <vector>

#include "rendering/animated_camera.h"

#include "geometry/light.h"

class RayTracerObject;
class SceneData;
class Scene {
  const int width;
  const int height;

  const float fov = 1.1;  // radians, guy in tutorial does pi/2

  const std::unique_ptr<RGBAImage> milkyWayBackground;
  const std::unique_ptr<TexturedSphereMaterial> milkyWayBackgroundSphere;

  std::vector<std::unique_ptr<Material>> materialLibrary2;
  std::map<std::string, std::unique_ptr<RGBAImage>> imageLibrary2;

  std::vector<std::unique_ptr<RayTracerObject>> objects;
  std::vector<Light> lights;

  AnimatedCamera eye;
  std::vector<Vec3f> frameBuffer;

 public:
  Scene(const SceneData& sceneData);
  std::vector<unsigned char> render() const;
  void update(float seconds);
};