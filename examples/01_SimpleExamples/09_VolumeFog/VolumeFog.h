//
// Created by wjl on 2023/1/29.
//

#ifndef LEARNVULKAN_VOLUMEFOG_H
#define LEARNVULKAN_VOLUMEFOG_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanPlane;

class VulkanTexture3D;

class VulkanTexture2D;

class UniformCamera;

class VolumeModelUniform;

class VulkanVertFragShader;

class VolumeFog : public ThirdPersonEngine {
public:
    VolumeFog() = default;

    virtual ~VolumeFog();

    void prepareFunctions() override;

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCameraUniform();

    void createVolumeRenderObject();

    void createVolumeTexture();

    void createTransferColor();

    void createModelUniform();

protected:
    std::shared_ptr<UniformCamera> m_cubeUniform = nullptr;
    std::shared_ptr<VulkanPlane> m_volumeRenderObject{};
    std::shared_ptr<VulkanTexture3D> m_volumeTexture{};
    std::shared_ptr<VulkanTexture2D> m_transferColor{};
    std::shared_ptr<VolumeModelUniform> m_volumeModelUniform{};

    std::shared_ptr<VulkanVertFragShader> m_volumeShader{};
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VOLUMEFOG_H
