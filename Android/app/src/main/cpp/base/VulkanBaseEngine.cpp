//
// Created by wjl on 21-2-23.
//

#include "VulkanBaseEngine.h"

#include "VulkanDescriptorSet.h"

BEGIN_NAMESPACE(VulkanEngine)
VulkanBaseEngine::VulkanBaseEngine() {}

VulkanBaseEngine::~VulkanBaseEngine() {}

void VulkanBaseEngine::prepare() {
    prepareBase();
    prepareDescriptorSets();
}

void VulkanBaseEngine::render() {}

void VulkanBaseEngine::processPrepareCallback() {}

void VulkanBaseEngine::prepareDescriptorSets() {
    m_VulkanDescriptorSet = new VulkanDescriptorSet(m_device,m_maxSets);
}

END_NAMESPACE(VulkanEngine)