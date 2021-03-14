//
// Created by wjl on 21-2-21.
//

#ifndef LEARNVULKAN_STATICCUBE_H
#define LEARNVULKAN_STATICCUBE_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class StaticCube : public ThirdPersonEngine {
public:
    StaticCube() = default;

    virtual ~StaticCube();

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

protected:
    VulkanCube *m_cube = nullptr;
    VulkanVertFragShader *m_cubeShader = nullptr;
    UniformCamera *m_cubeUniform = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_STATICCUBE_H
