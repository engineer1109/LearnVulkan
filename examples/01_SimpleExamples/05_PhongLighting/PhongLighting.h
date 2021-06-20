//
// Created by wjl on 21-4-5.
//

#ifndef LEARNVULKAN_PHONGLIGHTING_H
#define LEARNVULKAN_PHONGLIGHTING_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube;

class VulkanVertFragShader;

class UniformCamera;

class VulkanTexture2D;

class VulkanTextureCubeMap;

class ReflectParaBuffer;

class PhongLighting : public ThirdPersonEngine {
public:
    PhongLighting() = default;

    virtual ~PhongLighting();

    void prepareFunctions() override;

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

    void createSkybox();

    void createReflectParaBuffer();

    void changeReflectMode();

    void OnUpdateUIOverlay(vks::UIOverlay *overlay) override;

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

#endif //LEARNVULKAN_PHONGLIGHTING_H
