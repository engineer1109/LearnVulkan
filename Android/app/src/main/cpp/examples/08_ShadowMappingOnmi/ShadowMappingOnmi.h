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

class OnmiCamera;

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

    void createRoom();

    void createOnmiShadowFrameBuffer();

    void createDebugQuad();

    void updateCubeFace(uint32_t faceIndex, VkCommandBuffer &commandBuffer);

    void buildCommandBuffersBeforeMainRenderPass(VkCommandBuffer &cmd) override;

    void seeDebugQuad();

    void OnUpdateUIOverlay(vks::UIOverlay *overlay);

protected:
    std::vector<std::shared_ptr<VulkanCube>> m_cubes;
    std::shared_ptr<VulkanVertFragShader> m_cubeShader = nullptr;
    std::shared_ptr<UniformCamera> m_cubeUniform = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureA = nullptr;
    std::shared_ptr<VulkanTexture2D> m_cubeTextureB = nullptr;

    std::shared_ptr<VulkanCube> m_sky = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_skyShader = nullptr;
    std::shared_ptr<VulkanTextureCubeMap> m_skyTexture = nullptr;

    std::shared_ptr<ReflectParaBuffer> m_reflectParaBuffer = nullptr;

    std::shared_ptr<VulkanCube> m_room = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_roomShader = nullptr;

    std::shared_ptr<VulkanFrameBuffer> m_frameBuffer = nullptr;
    std::shared_ptr<VulkanTextureCubeMap> m_shadowMapTexture = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_shadowShader = nullptr;
    std::shared_ptr<OnmiCamera> m_onmiCamera = nullptr;

    std::shared_ptr<VulkanPlane> m_dubugPlane = nullptr;
    std::shared_ptr<VulkanVertFragShader> m_debugShader = nullptr;

    int m_cubeNum = 3;
    bool m_displayDebugQuad = false;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SHADOWMAPPINGONMI_H
