//
// Created by wjl on 21-3-22.
//

#ifndef LEARNVULKAN_TEXTURE2DCUBE_H
#define LEARNVULKAN_TEXTURE2DCUBE_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class Texture2dCube : public ThirdPersonEngine {
public:
    Texture2dCube() = default;

    ~Texture2dCube();

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

protected:
    VulkanCube *m_cube = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_TEXTURE2DCUBE_H
