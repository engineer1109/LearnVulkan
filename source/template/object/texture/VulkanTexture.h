//
// Created by wjl on 21-3-22.
//

#ifndef LEARNVULKAN_VULKANTEXTURE_H
#define LEARNVULKAN_VULKANTEXTURE_H

#include "render_common.h"
#include "vulkan_template.h"
#include "VkObject.h"

BEGIN_NAMESPACE(VulkanEngine)

interface VULKANENGINE_EXPORT_API VulkanTexture : public VkObject {
public:
    VulkanTexture() = default;

    virtual ~VulkanTexture() {
        destroy();
    }

    virtual void prepare() override {}

    virtual void update() override {}

    void setAddressMode(VkSamplerAddressMode mode) {
        m_addressMode[0] = mode;
        m_addressMode[1] = mode;
        m_addressMode[2] = mode;
    }

    void setAddressMode(const std::array<VkSamplerAddressMode, 3> &mode) {
        m_addressMode = mode;
    }

    vks::VulkanDevice *device = nullptr;
    VkImage image = VK_NULL_HANDLE;
    VkImageLayout imageLayout;
    VkDeviceMemory deviceMemory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    uint32_t width, height = 0;
    uint32_t depth = 0;
    uint32_t channels = 0;
    uint32_t mipLevels = 0;
    uint32_t layerCount = 0;
    size_t m_size = 0;
    std::array<VkSamplerAddressMode, 3> m_addressMode{VK_SAMPLER_ADDRESS_MODE_REPEAT,
                                                      VK_SAMPLER_ADDRESS_MODE_REPEAT,
                                                      VK_SAMPLER_ADDRESS_MODE_REPEAT};
    VkDescriptorImageInfo descriptor;

    VkSamplerAddressMode defaultAddressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    /** @brief Optional sampler to use with this texture */
    VkSampler sampler = VK_NULL_HANDLE;

    /** @brief Update image descriptor from current sampler, view and image layout */
    void updateDescriptor() {
        descriptor.sampler = sampler;
        descriptor.imageView = view;
        descriptor.imageLayout = imageLayout;
    }

    /** @brief Release all Vulkan resources held by this texture */
    void destroy() {
        vkDestroyImageView(device->logicalDevice, view, nullptr);
        vkDestroyImage(device->logicalDevice, image, nullptr);
        if (sampler) {
            vkDestroySampler(device->logicalDevice, sampler, nullptr);
        }
        vkFreeMemory(device->logicalDevice, deviceMemory, nullptr);
    }

    VkComponentMapping getComponentMapping(int channels) {
        switch (channels) {
            case 1:
                return {VK_COMPONENT_SWIZZLE_R};
                break;
            case 2:
                return {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G};
                break;
            case 3:
                return {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B};
                break;
            case 4:
                return {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
                break;
            default:
                return {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
                break;

        }
    }

    static size_t GetPixelSize(VkFormat format) {
        switch (format) {
            case VK_FORMAT_R8_UNORM:
                return 1;
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 4;
            case VK_FORMAT_R32_SFLOAT:
                return 4;
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 16;
            default:
                return 4;
        }
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANTEXTURE_H
