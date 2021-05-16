//
// Created by wjl on 21-5-17.
//

#ifndef LEARNVULKAN_SHADOWMAPPING_H
#define LEARNVULKAN_SHADOWMAPPING_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class VulkanTexture2D;

class VulkanTextureCubeMap;

class ShadowMapping : ThirdPersonEngine {
public:
    ShadowMapping() = default;

    ~ShadowMapping();

    void prepareFunctions() override;

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SHADOWMAPPING_H
