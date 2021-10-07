//
// Created by wjl on 21-3-22.
//

#ifndef LEARNVULKAN_TEXTURE2DCUBE_H
#define LEARNVULKAN_TEXTURE2DCUBE_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class VulkanTexture2D;

class Texture2dCube : public ThirdPersonEngine {
public:
    Texture2dCube() = default;

    virtual ~Texture2dCube();

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

protected:
    std::shared_ptr<VulkanCube> m_cube = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_cubeShader = nullptr;
    std::shared_ptr<UniformCamera> m_cubeUniform = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureA = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureB = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_TEXTURE2DCUBE_H
