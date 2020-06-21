#pragma once
#include <vector>
#include "vec3f.h"

class Face {
  //private attribute
    std::vector<Vec3f> vertices;
public:
    //constructor, takes in a list of 3D verticies
    Face(std::vector<Vec3f> vertices);

    //method get verrticies returns a reference to the verticies
    const std::vector<Vec3f>& getVertices() const;
};