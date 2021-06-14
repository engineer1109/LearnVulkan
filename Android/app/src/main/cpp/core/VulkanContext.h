//
// Created by wjl on 21-2-26.
//

#ifndef LEARNVULKAN_VULKANCONTEXT_H
#define LEARNVULKAN_VULKANCONTEXT_H

#include "render_common.h"

BEGIN_NAMESPACE(VulkanEngine)

struct VulkanContext {
    vks::VulkanDevice *vulkanDevice = nullptr;
    VkCommandBuffer copyCmd = VK_NULL_HANDLE;
    VkCommandPool cmdPool = VK_NULL_HANDLE;
    VkPipelineLayout *pPipelineLayout = nullptr;
    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
    uint32_t *pScreenWidth = nullptr;
    uint32_t *pScreenHeight = nullptr;

#ifdef __ANDROID__
    AAssetManager *m_asset = nullptr;
#endif

    VkDevice &getDevice(){
        return vulkanDevice->logicalDevice;
    }

    VkPhysicalDevice &getPhysicalDevice(){
        return vulkanDevice->physicalDevice;
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANCONTEXT_H
