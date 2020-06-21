#pragma once
#include <memory>
#include <nlohmann/json.hpp>

class SceneData;

namespace scene_reader {
std::unique_ptr<SceneData> readSceneFromJson(const nlohmann::json& jsonData);
}