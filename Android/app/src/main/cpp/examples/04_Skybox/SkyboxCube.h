//
// Created by wjl on 21-3-28.
//

#ifndef LEARNVULKAN_SKYBOXCUBE_H
#define LEARNVULKAN_SKYBOXCUBE_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class SkyboxCube : public ThirdPersonEngine  {
public:
    SkyboxCube() = default;

    virtual ~SkyboxCube();

    void prepareMyObjects() override;

    void buildMyObjects(VkCommandBuffer &cmd) override;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createCube();

    void createSkybox();
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SKYBOXCUBE_H
