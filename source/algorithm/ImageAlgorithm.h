//
// Created by wjl on 21-3-28.
//

#ifndef LEARNVULKAN_IMAGEALGORITHM_H
#define LEARNVULKAN_IMAGEALGORITHM_H

#include "base_template.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API ImageAlgorithm {
public:
    static void turnRGB2RGBA(uint8_t* inData, uint8_t* outData, int width, int height);

    static void mirrorX(uint8_t* data, int width, int height);

    static void mirror_X_Y(uint8_t* data, int width, int height);

    static void mirrorXY(uint8_t* data, int width, int height);

    static uint8_t &at(uint8_t* data, int x, int y, int c, int width, int height);
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_IMAGEALGORITHM_H
