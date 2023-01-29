//
// Created by wjl on 21-3-22.
//

#ifndef LEARNVULKAN_VULKANTEXTURE2D_H
#define LEARNVULKAN_VULKANTEXTURE2D_H

#include "VulkanTexture.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanTexture2D : public VulkanTexture {
public:
    VulkanTexture2D() = default;

    virtual ~VulkanTexture2D() = default;

#ifdef __ANDROID__
    void loadFromFile(std::string file, AAssetManager *asset, VkFormat format,
                      vks::VulkanDevice *device,
                      VkQueue copyQueue,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false);

    void loadFromFile(std::string file, VkFormat format,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false) {
        if (m_context) {
            loadFromFile(file, m_context->m_asset, format, m_context->vulkanDevice,
                         m_context->queue,
                         imageUsageFlags, imageLayout, forceLinear);
        }
    }
#else
    void loadFromFile(std::string file, VkFormat format,
                      vks::VulkanDevice *device,
                      VkQueue copyQueue,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false);

    void loadFromFile(std::string file, VkFormat format,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false) {
        if (m_context) {
            loadFromFile(file, format, m_context->vulkanDevice,
                         m_context->queue,
                         imageUsageFlags, imageLayout, forceLinear);
        }
    }

#endif

    void loadFromData(void *imgData,
                      uint32_t width,
                      uint32_t height,
                      uint32_t channels,
                      VkFormat format,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false);

};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANTEXTURE2D_H
