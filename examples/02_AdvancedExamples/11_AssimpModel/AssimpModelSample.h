//
// Created by wjl on 2021/6/20.
//

#ifndef LEARNVULKAN_ASSIMPMODELSAMPLE_H
#define LEARNVULKAN_ASSIMPMODELSAMPLE_H

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

class AssimpObject;

class ReflectParaBuffer;

class AssimpModelSample : public ThirdPersonEngine {
public:
    AssimpModelSample() = default;

    ~AssimpModelSample();

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

    void createDebugQuad();

    void createReflectParaBuffer();

    void buildCommandBuffersBeforeMainRenderPass(VkCommandBuffer &cmd) override;

    void seeDebugQuad();

    void OnUpdateUIOverlay(vks::UIOverlay *overlay) override;

protected:
    AssimpObject *m_assimpObject = nullptr;

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

    VulkanPlane *m_dubugPlane = nullptr;
    VulkanVertFragShader* m_debugShader = nullptr;

    ReflectParaBuffer* m_reflectParaBuffer = nullptr;

    bool m_seeDebug = false;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_ASSIMPMODELSAMPLE_H
