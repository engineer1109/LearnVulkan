//
// Created by wjl on 2021/7/18.
//

#ifndef LEARNVULKAN_SHADOWMAPPINGONMI_H
#define LEARNVULKAN_SHADOWMAPPINGONMI_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class VulkanTexture2D;

class VulkanTextureCubeMap;

class VulkanPlane;

class VulkanFrameBuffer;

class ReflectParaBuffer;

class ShadowMappingOnmi : public ThirdPersonEngine {
public:
    ShadowMappingOnmi() = default;

    virtual ~ShadowMappingOnmi();

    void prepareFunctions() override;

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

    void createSkybox();

    void createReflectParaBuffer();

protected:
    VulkanCube *m_cube = nullptr;
    VulkanVertFragShader *m_cubeShader = nullptr;
    UniformCamera *m_cubeUniform = nullptr;
    VulkanTexture2D *m_cubeTextureA = nullptr;
    VulkanTexture2D *m_cubeTextureB = nullptr;

    VulkanCube *m_sky = nullptr;
    VulkanVertFragShader *m_skyShader = nullptr;
    VulkanTextureCubeMap *m_skyTexture = nullptr;

    ReflectParaBuffer* m_reflectParaBuffer = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SHADOWMAPPINGONMI_H
