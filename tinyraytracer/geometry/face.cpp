#include "face.h"

//constructor
Face::Face(std::vector<Vec3f> vertices) : vertices(vertices) {}

//method get verrticies returns a reference to the verticies
const std::vector<Vec3f>& Face::getVertices() const { return vertices; }
