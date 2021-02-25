//
// Created by wjl on 21-2-26.
//

#ifndef LEARNVULKAN_VULKANPIPELINES_H
#define LEARNVULKAN_VULKANPIPELINES_H

#include "render_common.h"

#include "object/VkObject.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanPipelines {
public:
    VulkanPipelines() = default;
    ~VulkanPipelines() = default;

protected:
    VkDevice m_device;
    VkGraphicsPipelineCreateInfo m_pipelineCreateInfo;
    VkPipelineVertexInputStateCreateInfo *m_pVertexInputState;
    VkPipelineCache m_pipelineCache;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANPIPELINES_H
