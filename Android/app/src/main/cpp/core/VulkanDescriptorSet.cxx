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

void VulkanDescriptorSet::addBinding(uint32_t binding, VkDescriptorImageInfo *descriptorInfo,
                                     VkDescriptorType descriptorType, VkShaderStageFlags stageFlags,
                                     int descriptorIndex) {
    m_descriptorInfos.push_back(
            {binding, descriptorInfo, nullptr, descriptorType, stageFlags, descriptorIndex,
             DescriptorType::IMAGE});
}

void VulkanDescriptorSet::addBinding(uint32_t binding, VkDescriptorBufferInfo *descriptorInfo,
                                     VkDescriptorType descriptorType, VkShaderStageFlags stageFlags,
                                     int descriptorIndex) {
    m_descriptorInfos.push_back(
            {binding, nullptr, descriptorInfo, descriptorType, stageFlags, descriptorIndex,
             DescriptorType::BUFFER});
}

void VulkanDescriptorSet::GenPipelineLayout(VkPipelineLayout *pipelineLayout) {
    std::vector<VkDescriptorPoolSize> poolSizes;
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
    std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptorSets(m_descriptorSets.size());

    for (auto &descriptorInfo: m_descriptorInfos) {
        poolSizes.push_back(
                vks::initializers::descriptorPoolSize(descriptorInfo.descriptorType, 1));
        if (descriptorInfo.descriptorIndex == 0) {
            setLayoutBindings.push_back(
                    vks::initializers::descriptorSetLayoutBinding(descriptorInfo.descriptorType,
                                                                  descriptorInfo.stageFlags,
                                                                  descriptorInfo.binding));
        }
        if(descriptorInfo.type==DescriptorType::IMAGE){
            writeDescriptorSets[descriptorInfo.descriptorIndex].push_back(
                    vks::initializers::writeDescriptorSet(nullptr, descriptorInfo.descriptorType,
                                                          descriptorInfo.binding,
                                                          descriptorInfo.descriptorImageInfo));
        }
        else{
            writeDescriptorSets[descriptorInfo.descriptorIndex].push_back(
                    vks::initializers::writeDescriptorSet(nullptr, descriptorInfo.descriptorType,
                                                          descriptorInfo.binding,
                                                          descriptorInfo.descriptorBufferInfo));
        }
    }

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
            vks::initializers::descriptorPoolCreateInfo(static_cast<uint32_t>(poolSizes.size()),
                                                        poolSizes.data(),
                                                        m_descriptorSets.size());
    VK_CHECK_RESULT(
            vkCreateDescriptorPool(m_device, &descriptorPoolInfo, nullptr, &m_descriptorPool))

    VkDescriptorSetLayoutCreateInfo descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(
            setLayoutBindings.data(), static_cast<uint32_t>(setLayoutBindings.size()));
    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device, &descriptorLayout, nullptr, &m_descriptorSetLayout));

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo =
            vks::initializers::pipelineLayoutCreateInfo(&m_descriptorSetLayout, 1);

    VK_CHECK_RESULT(vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, pipelineLayout));

    for (int i=0; i<m_descriptorSets.size();i++){

        VkDescriptorSetAllocateInfo allocInfo =
                vks::initializers::descriptorSetAllocateInfo(m_descriptorPool, &m_descriptorSetLayout, 1);

        VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSets[i]));

        for (size_t j = 0; j < writeDescriptorSets[i].size(); j++) {
            writeDescriptorSets[i][j].dstSet = m_descriptorSets[i];
        }
        vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(writeDescriptorSets[i].size()), writeDescriptorSets[i].data(),
                               0, NULL);
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