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

class VulkanPlane;

class VulkanFrameBuffer;

class ShadowCamera;

class ShadowMapping : ThirdPersonEngine {
public:
    ShadowMapping() = default;

    ~ShadowMapping();

    void prepareFunctions() override;

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

    void createSkybox();

    void createPlane();

    void createShadowFrameBuffer();

    void buildCommandBuffers() override;

protected:
    VulkanCube *m_cube = nullptr;
    VulkanVertFragShader *m_cubeShader = nullptr;
    UniformCamera *m_cubeUniform = nullptr;
    VulkanTexture2D *m_cubeTextureA = nullptr;
    VulkanTexture2D *m_cubeTextureB = nullptr;

    VulkanCube *m_sky = nullptr;
    VulkanVertFragShader *m_skyShader = nullptr;
    VulkanTextureCubeMap *m_skyTexture = nullptr;

    VulkanPlane *m_plane = nullptr;
    VulkanVertFragShader *m_planeShader = nullptr;
    VulkanTexture2D *m_planeTexture = nullptr;

    VulkanFrameBuffer *m_frameBuffer = nullptr;
    VulkanVertFragShader *m_shadowShader = nullptr;
    ShadowCamera* m_shadowCamera = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SHADOWMAPPING_H
