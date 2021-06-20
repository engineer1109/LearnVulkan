//
// Created by wjl on 21-2-23.
//

#ifndef LEARNVULKAN_VULKANDESCRIPTORSET_H
#define LEARNVULKAN_VULKANDESCRIPTORSET_H

#include "render_common.h"
#include "vulkan_macro.h"

#include "VulkanTools.h"
#include "VulkanInitializers.hpp"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanDescriptorSet {
public:
    enum class DescriptorType{
        IMAGE = 0,
        BUFFER = 1,
    };

    struct DescriptorInfo {
        uint32_t binding = 0;
        VkDescriptorImageInfo *descriptorImageInfo = nullptr;
        VkDescriptorBufferInfo *descriptorBufferInfo = nullptr;
        VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        int descriptorIndex = 0;
        DescriptorType type = DescriptorType::IMAGE;
    };
public:
    VulkanDescriptorSet(VkDevice device, int maxSets = 1);

    ~VulkanDescriptorSet();

    void addBinding(uint32_t binding, VkDescriptorImageInfo *descriptorInfo,
                    VkDescriptorType descriptorType,
                    VkShaderStageFlags stageFlags,
                    int descriptorIndex);

    void addBinding(uint32_t binding, VkDescriptorBufferInfo *descriptorInfo,
                    VkDescriptorType descriptorType,
                    VkShaderStageFlags stageFlags,
                    int descriptorIndex);

    void GenPipelineLayout(VkPipelineLayout *pipelineLayout);

    VkDescriptorSet &get(int index);

    size_t getSize();

protected:
    VkDevice m_device = VK_NULL_HANDLE;

    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<DescriptorInfo> m_descriptorInfos;

    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANDESCRIPTORSET_H
