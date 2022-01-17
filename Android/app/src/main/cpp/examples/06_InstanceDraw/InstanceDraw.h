//
// Created by wjl on 21-4-11.
//

#ifndef LEARNVULKAN_INSTANCEDRAW_H
#define LEARNVULKAN_INSTANCEDRAW_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class VulkanTexture2D;

class VulkanTextureCubeMap;

class DefaultInstanceDescription;

class InstanceCube;

class SpeedBuffer;

class InstanceDraw : public ThirdPersonEngine {
public:
    InstanceDraw() = default;

    virtual ~InstanceDraw();

    void prepareFunctions() override;

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void createInstanceDescription();

    void setDescriptorSet();

    void createPipelines();

    void createInstanceCube();

    void createSkybox();

    void createSpeedBuffer();

private:
    std::shared_ptr<DefaultInstanceDescription> m_instanceDescription = nullptr;

    std::shared_ptr<UniformCamera> m_cubeUniform = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureA = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureB = nullptr;

    std::shared_ptr<VulkanCube> m_sky = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_skyShader = nullptr;
    std::shared_ptr<VulkanTextureCubeMap> m_skyTexture = nullptr;

    std::shared_ptr<InstanceCube> m_instanceCube = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_instanceCubeShader = nullptr;

    std::shared_ptr<SpeedBuffer> m_speedBuffer = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_INSTANCEDRAW_H
