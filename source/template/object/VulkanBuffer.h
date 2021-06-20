//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_VULKANBUFFER_H
#define LEARNVULKAN_VULKANBUFFER_H

#include "VkObject.h"
#include "VulkanContext.h"
#include "VulkanBuffer.hpp"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanBuffer : public VkObject {
public:
    VulkanBuffer() = default;
    virtual ~VulkanBuffer();

    virtual void prepare() override;

    virtual void update() override;

    virtual void prepareUniformBuffers() = 0;

    virtual void updateUniformBuffers() = 0;

public:
    vks::Buffer m_uniformBuffer;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANBUFFER_H
