//
// Created by wjl on 21-2-26.
//

#ifndef LEARNVULKAN_VKOBJECT_H
#define LEARNVULKAN_VKOBJECT_H

#include "render_common.h"
#include "VulkanDevice.hpp"

BEGIN_NAMESPACE(VulkanEngine)

class VkObject{
public:
    VkObject() = default;

    virtual ~VkObject() = default;

    virtual void prepare() = 0;

    virtual void update() = 0;

protected:
    vks::VulkanDevice *m_vulkanDevice = nullptr;
    VkCommandBuffer m_copyCmd;
    VkCommandPool m_cmdPool;
    VkPipelineLayout *m_pPipelineLayout = nullptr;
    VkPipelineCache m_pipelineCache;
    VkRenderPass m_renderPass;
    VkQueue m_queue;
    uint32_t *m_pScreenWidth = nullptr;
    uint32_t *m_pScreenHeight = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VKOBJECT_H
