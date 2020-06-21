#include "light_data.h"

LightData::LightData(Vec3f location, float intensity) : location(location), intensity(intensity) {}
const Vec3f& LightData::getLocation() const { return location; }
float LightData::getIntensity() const { return intensity; }