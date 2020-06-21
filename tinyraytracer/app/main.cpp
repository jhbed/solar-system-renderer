#include <geometry/light.h>
#include <geometry/sphere2.h>
#include <geometry/transform.h>
#include <geometry/vec3f.h>
#include <lodepng/lodepng.h>
#include <rendering/clouds_material.h>
#include <rendering/color_material.h>
#include <rendering/material.h>
#include <rendering/rgba_image.h>
#include <rendering/scene.h>
#include <rendering/textured_sphere_material.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>

#include "data/scene_data.h"
#include "data/scene_reader.h"
#include "geometry/mat4f.h"
#include <chrono>

int main(int argc, char* argv[]) {
  int frameStart = 0;
  int frameEnd = 1;
  float timeStep = 0.01f;
  std::string filename = "./assets/scripts/test3.json";
  

  for (int i = 1; i < argc; i += 2) {
    std::cout << argv[i] << std::endl;
    std::string arg(argv[i]);
    if (arg.compare("-fs") == 0) {
      std::istringstream iss(argv[i + 1]);
      iss >> frameStart;
    }
    if (arg.compare("-fe") == 0) {
      std::istringstream iss(argv[i + 1]);
      iss >> frameEnd;
    }

    if (arg.compare("-scenefile") == 0) {
      filename = argv[i + 1];
    }
  }

  int beginning = filename.find_last_of("/");
  std::string frameName = filename.substr(beginning+1, filename.find_last_of(".")-beginning-1);

  std::ifstream stream(filename);
  nlohmann::json scriptJson;
  stream >> scriptJson;
  std::unique_ptr<SceneData> sceneData =
      scene_reader::readSceneFromJson(scriptJson);

  // create a scene object on the stack
  Scene scene(*sceneData.get());
  for (int i = 0; i < frameStart; i++) {
    scene.update(timeStep);
  }

  
  for (int frame = frameStart; frame < frameEnd; frame++) {

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<unsigned char> img_data = scene.render();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end-start);

    std::cout << "Render time: " << duration.count() << std::endl;

    std::ostringstream oss;
    oss << "./frames/" << frameName << ".png";
    lodepng::encode(oss.str(), img_data, sceneData->getWidth(),
                    sceneData->getHeight());
    scene.update(timeStep);
  }
  return 0;
}