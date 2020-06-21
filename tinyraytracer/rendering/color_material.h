#pragma once
#include <geometry/vec3f.h>
#include "material.h"


class ColorMaterial : public Material {

    //shinyness
    float specularity;
    Vec3f color;
    //how much ambient light gets reflected, how much other stuff gets reflected
    Vec3f albedo;

    public:
        ColorMaterial(const float specularity, const Vec3f color, const Vec3f albedo);
        virtual float getSpecularity() const override;
        virtual Vec3f getColor(const Vec3f &collisionPoint, float diffuseLightIntensity) const override;
        virtual Vec3f getAlbedo() const override;
};