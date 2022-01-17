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
    std::shared_ptr<VulkanCube> m_cube = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_cubeShader = nullptr;
    std::shared_ptr<UniformCamera> m_cubeUniform = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureA = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureB = nullptr;

    std::shared_ptr<VulkanCube> m_sky = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_skyShader = nullptr;
    std::shared_ptr<VulkanTextureCubeMap> m_skyTexture = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SKYBOXCUBE_H
