//
// Created by wjl on 21-2-23.
//

#ifndef LEARNVULKAN_VULKANDESCRIPTORSET_H
#define LEARNVULKAN_VULKANDESCRIPTORSET_H

#include "render_common.h"
#include "vulkan_macro.h"

#include "VulkanInitializers.hpp"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanDescriptorSet {
public:
    struct DescriptorInfo {
        uint32_t binding = 0;
        VkDescriptorImageInfo *descriptorInfo = nullptr;
        VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        int descriptorIndex = 0;
    };
public:
    VulkanDescriptorSet(VkDevice device, int maxSets = 1);

    ~VulkanDescriptorSet();

    void
    addBinding(uint32_t binding, VkDescriptorImageInfo *descriptor, VkDescriptorType descriptorType,
               VkShaderStageFlags stageFlags,
               int descriptorIndex);

    VkDescriptorSet &get(int index);

    size_t getSize();

protected:
    VkDevice m_device = VK_NULL_HANDLE;

    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<DescriptorInfo> m_descriptorInfos;

    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

    std::vector<VkDescriptorPoolSize> m_poolSizes;
    std::vector<VkDescriptorSetLayoutBinding> m_setLayoutBindings;
    std::vector<std::vector<VkWriteDescriptorSet>> m_writeDescriptorSets;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANDESCRIPTORSET_H
