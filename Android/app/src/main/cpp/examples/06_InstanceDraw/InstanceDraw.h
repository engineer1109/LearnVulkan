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
    DefaultInstanceDescription* m_instanceDescription = nullptr;

    UniformCamera *m_cubeUniform = nullptr;
    VulkanTexture2D *m_cubeTextureA = nullptr;
    VulkanTexture2D *m_cubeTextureB = nullptr;

    VulkanCube *m_sky = nullptr;
    VulkanVertFragShader *m_skyShader = nullptr;
    VulkanTextureCubeMap *m_skyTexture = nullptr;

    InstanceCube* m_instanceCube = nullptr;
    VulkanVertFragShader* m_instanceCubeShader = nullptr;

    SpeedBuffer *m_speedBuffer = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_INSTANCEDRAW_H
