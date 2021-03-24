//
// Created by wjl on 21-3-22.
//

#ifndef LEARNVULKAN_VULKANTEXTURE_H
#define LEARNVULKAN_VULKANTEXTURE_H

#include "render_common.h"
#include "vulkan_template.h"
#include "VulkanDevice.hpp"

BEGIN_NAMESPACE(VulkanEngine)

interface VulkanTexture {
public:
    VulkanTexture() = default;
    virtual ~VulkanTexture(){
        destroy();
    }

    vks::VulkanDevice *device = nullptr;
    VkImage image = VK_NULL_HANDLE;
    VkImageLayout imageLayout;
    VkDeviceMemory deviceMemory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    uint32_t width, height = 0;
    uint32_t channels = 0;
    uint32_t mipLevels = 0;
    uint32_t layerCount = 0;
    size_t m_size = 0;
    VkDescriptorImageInfo descriptor;

    /** @brief Optional sampler to use with this texture */
    VkSampler sampler = VK_NULL_HANDLE;

    /** @brief Update image descriptor from current sampler, view and image layout */
    void updateDescriptor()
    {
        descriptor.sampler = sampler;
        descriptor.imageView = view;
        descriptor.imageLayout = imageLayout;
    }

    /** @brief Release all Vulkan resources held by this texture */
    void destroy()
    {
        vkDestroyImageView(device->logicalDevice, view, nullptr);
        vkDestroyImage(device->logicalDevice, image, nullptr);
        if (sampler)
        {
            vkDestroySampler(device->logicalDevice, sampler, nullptr);
        }
        vkFreeMemory(device->logicalDevice, deviceMemory, nullptr);
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANTEXTURE_H
