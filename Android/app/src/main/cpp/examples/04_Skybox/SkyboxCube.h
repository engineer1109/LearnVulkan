//
// Created by wjl on 21-3-28.
//

#ifndef LEARNVULKAN_SKYBOXCUBE_H
#define LEARNVULKAN_SKYBOXCUBE_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class VulkanTexture2D;

class VulkanTextureCubeMap;

class SkyboxCube : public ThirdPersonEngine  {
public:
    SkyboxCube() = default;

    virtual ~SkyboxCube();

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

    void createSkybox();

protected:
    VulkanCube *m_cube = nullptr;
    VulkanVertFragShader *m_cubeShader = nullptr;
    UniformCamera *m_cubeUniform = nullptr;
    VulkanTexture2D *m_cubeTextureA = nullptr;
    VulkanTexture2D *m_cubeTextureB = nullptr;

    VulkanCube *m_sky = nullptr;
    VulkanVertFragShader *m_skyShader = nullptr;
    UniformCamera *m_skyUniform = nullptr;
    VulkanTextureCubeMap *m_skyTexture = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SKYBOXCUBE_H
