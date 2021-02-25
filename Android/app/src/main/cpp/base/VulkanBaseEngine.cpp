//
// Created by wjl on 21-2-23.
//

#include "VulkanBaseEngine.h"

#include "VulkanDescriptorSet.h"
#include "VulkanVertexDescriptions.h"

BEGIN_NAMESPACE(VulkanEngine)
VulkanBaseEngine::VulkanBaseEngine() {}

VulkanBaseEngine::~VulkanBaseEngine() {}

void VulkanBaseEngine::prepare() {
    prepareBase();
    prepareDescriptorSets();
    prepareVertexDescriptions();
}

void VulkanBaseEngine::render() {}

void VulkanBaseEngine::processPrepareCallback() {}

void VulkanBaseEngine::prepareDescriptorSets() {
    m_VulkanDescriptorSet = new VulkanDescriptorSet(m_device,m_maxSets);
}

void VulkanBaseEngine::prepareVertexDescriptions(){
    m_VulkanVertexDescriptions = new VulkanVertexDescriptions();
    m_VulkanVertexDescriptions->GenerateTexVec4Descriptions();
}

END_NAMESPACE(VulkanEngine)