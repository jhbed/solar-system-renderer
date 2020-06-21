#pragma once
#include <vector>

#include "light_data.h"
#include "raytracer_object_data.h"

class SceneData {
  int width;
  int height;
  Vec3f cameraLocation;
  Vec3f cameraRotation;
  std::vector<RaytracerObjectData> raytracerObjects;
  std::vector<LightData> lights;
  float fov;

 public:
  SceneData(int width, int height,
            std::vector<RaytracerObjectData>& raytracerObjects, std::vector<LightData>& lights,
            Vec3f& cameraLocation, Vec3f& getCameraRotation, float fov);
  const std::vector<RaytracerObjectData>& getObjects() const;
  const std::vector<LightData>& getLights() const;
  int getWidth() const;
  int getHeight() const;
  float getFov() const;
  const Vec3f& getCameraLocation() const;
  const Vec3f& getCameraRotation() const;
};