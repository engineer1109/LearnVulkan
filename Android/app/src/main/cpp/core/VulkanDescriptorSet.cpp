//
// Created by wjl on 21-2-23.
//

#include "VulkanDescriptorSet.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanDescriptorSet::VulkanDescriptorSet(VkDevice device, int maxSets) {
    m_device = device;
    m_descriptorSets.resize(maxSets);
}

VulkanDescriptorSet::~VulkanDescriptorSet() {
    for (auto &descriptorSet : m_descriptorSets) {
        VK_SAFE_DELETE(descriptorSet,
                       vkFreeDescriptorSets(m_device, m_descriptorPool, 1, &descriptorSet));
    }
    VK_SAFE_DELETE(m_descriptorPool, vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr));
    VK_SAFE_DELETE(m_descriptorSetLayout,
                   vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr));
}

void VulkanDescriptorSet::addBinding(uint32_t binding, VkDescriptorImageInfo *descriptor, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, int descriptorIndex) {
    if(descriptorIndex<m_descriptorSets.size()){
        const uint32_t descriptorCount = 1;
        m_poolSizes.push_back(vks::initializers::descriptorPoolSize(descriptorType, descriptorCount));
        m_setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(descriptorType, stageFlags, binding));
        m_writeDescriptorSets[descriptorIndex].push_back(vks::initializers::writeDescriptorSet(nullptr, descriptorType, binding, descriptor));
    }
    else{
        LOGI("VulkanDescriptorSet|addBinding| descriptorIndex out of size.");
        return;
    }
}

VkDescriptorSet &VulkanDescriptorSet::get(int index) {
    if (index < m_descriptorSets.size()) {
        return m_descriptorSets[index];
    } else {
        LOGI("VulkanDescriptorSet|get| DescriptorSets Out of Range");
        return m_descriptorSets[0];
    }
}

size_t VulkanDescriptorSet::getSize() {
    return m_descriptorSets.size();
}

END_NAMESPACE(VulkanEngine)