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

class ReflectParaBuffer;

class ShadowMapping : public ThirdPersonEngine {
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

    void createDebugQuad();

    void createReflectParaBuffer();

    void buildCommandBuffersBeforeMainRenderPass(VkCommandBuffer &cmd) override;

    void seeDebugQuad();

    void OnUpdateUIOverlay(vks::UIOverlay *overlay) override;

    void setReflect(float value);

protected:
    std::shared_ptr<VulkanCube> m_cube = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_cubeShader = nullptr;
    std::shared_ptr<UniformCamera> m_cubeUniform = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureA = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureB = nullptr;

    std::shared_ptr<VulkanCube> m_sky = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_skyShader = nullptr;
    std::shared_ptr<VulkanTextureCubeMap> m_skyTexture = nullptr;

    std::shared_ptr<VulkanPlane> m_plane = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_planeShader = nullptr;
    std::shared_ptr<VulkanTexture2D> m_planeTexture = nullptr;

    std::shared_ptr<VulkanFrameBuffer> m_frameBuffer = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_shadowShader = nullptr;
    std::shared_ptr<ShadowCamera> m_shadowCamera = nullptr;

    std::shared_ptr<VulkanPlane> m_dubugPlane = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_debugShader = nullptr;

    std::shared_ptr<ReflectParaBuffer> m_reflectParaBuffer = nullptr;

    bool m_seeDebug = false;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SHADOWMAPPING_H
