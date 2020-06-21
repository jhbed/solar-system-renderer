#pragma once
#include <vector>
#include "geometry/vec3f.h"

namespace frame_effects {
std::vector<Vec3f> depthOfField(const std::vector<float>& depthBuffer,
                                const std::vector<Vec3f>& img, int width,
                                int height, float focalDepth,
                                float focusDistance);

Vec3f blurPixel(int i, int j, const std::vector<Vec3f>& img, int width,
                int height, float sigma, int kSize);

}  // namespace frame_effects
