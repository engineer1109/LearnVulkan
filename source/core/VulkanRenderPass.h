//
// Created by wjl on 21-5-23.
//

#ifndef LEARNVULKAN_VULKANRENDERPASS_H
#define LEARNVULKAN_VULKANRENDERPASS_H

#include "render_common.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanRenderPass {
public:
    VulkanRenderPass() = default;

    virtual ~VulkanRenderPass();

    void setDevice(const VkDevice &device) { m_device = device; }

    void setFormat(VkFormat format) { m_format = format; }

    void setDepthFormat(VkFormat format) { m_depthFormat = format; }

    void createColorDepthPass();

    void createDepthPass();

    VkRenderPass &get() { return m_renderPass; }

protected:
    VkDevice m_device = VK_NULL_HANDLE;
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkFormat m_format = VK_FORMAT_R32_SFLOAT;
    VkFormat m_depthFormat = VK_FORMAT_D16_UNORM;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANRENDERPASS_H
