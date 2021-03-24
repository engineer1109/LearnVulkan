//
// Created by wjl on 21-3-22.
//

#ifndef LEARNVULKAN_VULKANTEXTURE2D_H
#define LEARNVULKAN_VULKANTEXTURE2D_H

#include "VulkanTexture.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanTexture2D : public VulkanTexture {
public:
    VulkanTexture2D() = default;

    virtual ~VulkanTexture2D() = default;

    void loadFromFile(std::string file, AAssetManager *asset, VkFormat format,
                      vks::VulkanDevice *device,
                      VkQueue copyQueue,
                      VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
                      VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      bool forceLinear = false);
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANTEXTURE2D_H
