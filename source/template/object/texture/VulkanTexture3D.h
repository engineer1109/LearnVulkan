//
// Created by wjl on 2023/1/29.
//

#ifndef LEARNVULKAN_VULKANTEXTURE3D_H
#define LEARNVULKAN_VULKANTEXTURE3D_H

#include "VulkanTexture.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanTexture3D : public VulkanTexture {
public:
    VulkanTexture3D() = default;

    virtual ~VulkanTexture3D() = default;

    void loadFromData(void *imgData,
                      uint32_t width,
                      uint32_t height,
                      uint32_t depth,
                      VkFormat format,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false);
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANTEXTURE3D_H
