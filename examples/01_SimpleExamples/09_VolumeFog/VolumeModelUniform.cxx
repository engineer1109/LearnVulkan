//
// Created by wjl on 2023/1/29.
//

#include "VolumeModelUniform.h"

BEGIN_NAMESPACE(VulkanEngine)

void VolumeModelUniform::prepareUniformBuffers() {
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_uniformBuffer, sizeof(m_model), &m_model));
    VK_CHECK_RESULT(m_uniformBuffer.map());
    updateUniformBuffers();
}

void VolumeModelUniform::updateUniformBuffers() {
    memcpy(m_uniformBuffer.mapped, &m_model, sizeof(m_model));
}

END_NAMESPACE(VulkanEngine)