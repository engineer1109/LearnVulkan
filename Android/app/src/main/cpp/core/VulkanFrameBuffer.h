//
// Created by wjl on 21-5-23.
//

#ifndef LEARNVULKAN_VULKANFRAMEBUFFER_H
#define LEARNVULKAN_VULKANFRAMEBUFFER_H

#include "render_common.h"
#include "vulkan_template.h"

#include "VulkanDevice.hpp"
#include "VulkanRenderPass.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanFrameBuffer {
public:
    VulkanFrameBuffer() = default;

    virtual ~VulkanFrameBuffer();

    void create();

    void setSize(int width, int height) {
        m_width = width;
        m_height = height;
    }

    void setVulkanDevice(vks::VulkanDevice *vulkanDevice) {
        m_vulkanDevice = vulkanDevice;
        m_device = m_vulkanDevice->logicalDevice;
    }

    void setFormat(VkFormat format) {
        m_format = format;
    }

    VulkanRenderPass *&getRenderPass() { return m_renderPass; }

    VkFramebuffer &get() { return m_frameBuffer; }

    VkDescriptorImageInfo &getDescriptor() { return m_descriptor; }

    int getWidth() const {return m_width;}

    int getHeight() const {return m_height;}

protected:
    int m_width = 2048;
    int m_height = 2048;
    VkDevice m_device = VK_NULL_HANDLE;
    VkFormat m_format = VK_FORMAT_R8G8B8A8_UNORM;
    vks::VulkanDevice *m_vulkanDevice = nullptr;
    VulkanRenderPass *m_renderPass = nullptr;
    VkFramebuffer m_frameBuffer = VK_NULL_HANDLE;

    VkImageView m_view = VK_NULL_HANDLE;
    VkImage m_image = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    VkSampler m_depthSampler = VK_NULL_HANDLE;
    VkDescriptorImageInfo m_descriptor;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANFRAMEBUFFER_H
