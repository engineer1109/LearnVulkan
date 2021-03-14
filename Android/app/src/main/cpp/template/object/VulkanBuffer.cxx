//
// Created by wjl on 21-2-28.
//

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanBuffer::~VulkanBuffer(){
    m_uniformBuffer.unmap();
    m_uniformBuffer.destroy();
}

void VulkanBuffer::prepare() {
    prepareUniformBuffers();
}

void VulkanBuffer::update() {
    updateUniformBuffers();
}

END_NAMESPACE(VulkanEngine)
