//
// Created by wjl on 21-3-28.
//

#ifndef LEARNVULKAN_VULKANTEXTURECUBEMAP_H
#define LEARNVULKAN_VULKANTEXTURECUBEMAP_H

#include "VulkanTexture.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanTextureCubeMap : public VulkanTexture {
public:
    VulkanTextureCubeMap() = default;
    virtual ~VulkanTextureCubeMap() = default;

    void loadFromFile(std::vector<std::string> files, AAssetManager *asset, VkFormat format,
                      vks::VulkanDevice *device,
                      VkQueue copyQueue,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false);

    void loadFromFile(std::vector<std::string> files, VkFormat format,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false) {
        if (m_context) {
            loadFromFile(files, m_context->m_asset, format, m_context->vulkanDevice,
                         m_context->queue,
                         imageUsageFlags, imageLayout, forceLinear);
        }
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANTEXTURECUBEMAP_H
