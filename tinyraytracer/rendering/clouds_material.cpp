#include "clouds_material.h"
#include "rgba_image.h"
#include "math.h"

const float PI = 3.1415926f;
const float twoPi = PI*2;

CloudsMaterial::CloudsMaterial(const float specularity, const RGBAImage &texture, 
                                               const Vec3f &albedo, 
                                               const Vec3f &sphereCenter): specularity(specularity), 
                                               texture(texture), albedo(albedo), sphereCenter(sphereCenter) {}

float CloudsMaterial::getSpecularity() const {
    return specularity;
}
Vec3f CloudsMaterial::getColor(const Vec3f &collisionPoint, float diffuseLightIntensity) const {

    Vec3f collisionPointOrigin = collisionPoint - sphereCenter;
    float angle = atan2(collisionPointOrigin.z, collisionPointOrigin.x);
   
    while (angle > twoPi) {
        angle -= twoPi;
    }
    float xPercent = 1.0f - (angle / twoPi);

    Vec3f normCollision = collisionPointOrigin.normalize();

    //add one to get 0-2 scale, divide by 2 to get 0-1 scale, which is the scale 
    //we use to map to our picture
    //float yPercent = (1.0f - (normCollision.y + 1.0f) / 2.0f);
    float yPercent;
    yPercent = asin(normCollision.y) / (PI*0.5f);
    yPercent = (1.0f - (yPercent + 1.0f) / 2.0f);

     Vec3f color = texture.getPixelNormalized(xPercent, yPercent);
     
     return color;
    //return texture.getPixelNormalized(0.5, 0.5);
}
Vec3f CloudsMaterial::getAlbedo() const {
    return albedo;
}