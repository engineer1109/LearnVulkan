//
// Created by wjl on 21-5-5.
//

#ifndef LEARNVULKAN_SPEEDBUFFER_H
#define LEARNVULKAN_SPEEDBUFFER_H

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

class SpeedBuffer : public VulkanBuffer {
public:
    struct SpeedData{
        glm::vec4 speed;
    } m_speedData;
public:
    SpeedBuffer() = default;
    ~SpeedBuffer() = default;

    virtual void prepareUniformBuffers() override;
    virtual void updateUniformBuffers() override ;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_SPEEDBUFFER_H
