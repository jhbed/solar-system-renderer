#include "scene_reader.h"

#include <math.h>
#include <rendering/util.h>

#include <iostream>
#include <map>
#include <vector>

#include "light_data.h"
#include "raytracer_object_data.h"
#include "scene_data.h"

namespace scene_reader {

namespace {

std::map<std::string, RaytracerObjectType> raytracerObjectMap = {
    {"sphere", RaytracerObjectType::sphere},
    {"ring", RaytracerObjectType::ring},
    {"empty", RaytracerObjectType::empty},
};

RaytracerObjectType readType(const nlohmann::json& jsonData) {
  return raytracerObjectMap.at(jsonData.get<std::string>());
}
Vec3f readVec3f(const nlohmann::json& jsonData) {
  return Vec3f(jsonData["x"].get<float>(), jsonData["y"].get<float>(),
               jsonData["z"].get<float>());
}

RaytracerObjectData readObject(const nlohmann::json& objectJson) {
  RaytracerObjectType objectType = readType(objectJson["type"]);
  Vec3f center = readVec3f(objectJson["center"]);
  float radius = objectJson["radius"].get<float>();
  Vec3f rotation = readVec3f(objectJson["rotation"]);
  std::string imageFileName = objectJson["imageFileName"].get<std::string>();

  Vec3f velocity = readVec3f(objectJson["velocity"]);
  Vec3f angularVelocity = readVec3f(objectJson["angularVelocity"]);

  std::vector<RaytracerObjectData> children;

  if (objectJson.contains("children")) {
    for (auto& child : objectJson["children"]) {
      children.push_back(readObject(child));
    }
  }

  RaytracerObjectData obj(objectType, center, imageFileName, radius, rotation,
                          velocity, angularVelocity, children);

  if (objectType == RaytracerObjectType::ring) {
    obj.setInnerRadius(objectJson["innerRadius"].get<float>());
  }

  return obj;
}

}  // namespace

std::unique_ptr<SceneData> readSceneFromJson(const nlohmann::json& jsonData) {
  std::vector<RaytracerObjectData> objects;
  std::vector<LightData> lights;

  int width = jsonData["frameSize"]["width"].get<int>();
  int height = jsonData["frameSize"]["height"].get<int>();
  Vec3f cameraLocation = readVec3f(jsonData["cameraLocation"]);
  Vec3f cameraRotation = readVec3f(jsonData["cameraRotation"]);
  float fov = jsonData["fov"].get<float>();

  for (auto& objectJson : jsonData["objects"]) {
    RaytracerObjectData obj = readObject(objectJson);
    objects.push_back(obj);
  }

  for (auto& objectJson : jsonData["lights"]) {
    LightData lightData(readVec3f(objectJson["location"]),
                        objectJson["intensity"].get<float>());
    lights.push_back(lightData);
  }

  return std::unique_ptr<SceneData>(new SceneData(
      width, height, objects, lights, cameraLocation, cameraRotation, fov));
}
}  // namespace scene_reader
