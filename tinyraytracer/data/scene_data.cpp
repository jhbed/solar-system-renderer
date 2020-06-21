#include "scene_data.h"
#include "light_data.h"

SceneData::SceneData(int width, int height,
                     std::vector<RaytracerObjectData>& raytracerObjects, std::vector<LightData>& lights,
                     Vec3f& cameraLocation, Vec3f& cameraRotation, float fov)
    : width(width),
      height(height),
      raytracerObjects(raytracerObjects),
      lights(lights),
      cameraLocation(cameraLocation),
      cameraRotation(cameraRotation),
      fov(fov) {}

const std::vector<RaytracerObjectData>& SceneData::getObjects() const {
  return raytracerObjects;
}

const std::vector<LightData>& SceneData::getLights() const {
  return lights;
}

int SceneData::getWidth() const { return width; }
int SceneData::getHeight() const { return height; }
float SceneData::getFov() const { return fov; }
const Vec3f& SceneData::getCameraLocation() const { return cameraLocation; }
const Vec3f& SceneData::getCameraRotation() const { return cameraRotation; }