#pragma once
#include "vec3f.h"

struct Light {

    Vec3f position;
    float intensity;

    Light(const Vec3f &p, const float &i);
};