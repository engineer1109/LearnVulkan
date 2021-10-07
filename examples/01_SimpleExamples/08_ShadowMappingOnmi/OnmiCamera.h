//
// Created by wjl on 2021/8/16.
//

#ifndef LEARNVULKAN_ONMICAMERA_H
#define LEARNVULKAN_ONMICAMERA_H

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

class OnmiCamera : public VulkanBuffer {
public:
    struct {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
        glm::vec4 lightpos;
    }m_mvp;
public:
    OnmiCamera() = default;

    virtual void prepareUniformBuffers() override;
    virtual void updateUniformBuffers() override ;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_ONMICAMERA_H
