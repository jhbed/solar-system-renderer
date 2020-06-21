#include "color_material.h"

ColorMaterial::ColorMaterial(const float specularity, 
                   const Vec3f color, 
                   const Vec3f albedo): specularity(specularity), color(color), albedo(albedo) {}

float ColorMaterial::getSpecularity() const {
    return specularity;
}
Vec3f ColorMaterial::getColor(const Vec3f &collisionPoint, float diffuseLightIntensity) const {
    return color;
}
Vec3f ColorMaterial::getAlbedo() const {
    return albedo;
}