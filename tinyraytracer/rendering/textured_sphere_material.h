#pragma once
#include <geometry/vec3f.h>
#include "material.h"

class RGBAImage;

class TexturedSphereMaterial : public Material {

    //shinyness
    float specularity;
    const RGBAImage &texture;
    //how much ambient light gets reflected, how much other stuff gets reflected
    Vec3f albedo;
    Vec3f sphereCenter;

    public:
        TexturedSphereMaterial(const float specularity, const RGBAImage &texture, const Vec3f &albedo, const Vec3f &sphereCenter);
        virtual float getSpecularity() const override;
        virtual Vec3f getColor(const Vec3f &collisionPoint, float diffuseLightIntensity) const override;
        virtual Vec3f getColorScaled(const Vec3f &collisionPoint, int scaleFactor) const override;
        virtual Vec3f getAlbedo() const override;
};