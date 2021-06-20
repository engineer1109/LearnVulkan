//
// Created by wjl on 21-5-5.
//

#include "SpeedBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

void SpeedBuffer::prepareUniformBuffers() {
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_uniformBuffer, sizeof(m_speedData), &m_speedData));
    VK_CHECK_RESULT(m_uniformBuffer.map());
    updateUniformBuffers();
}

void SpeedBuffer::updateUniformBuffers() {
    memcpy(m_uniformBuffer.mapped, &m_speedData, sizeof(m_speedData));
}


END_NAMESPACE(VulkanEngine)