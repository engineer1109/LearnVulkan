//
// Created by wjl on 2021/8/16.
//

#include "OnmiCamera.h"

#include <glm/gtc/matrix_transform.hpp>

BEGIN_NAMESPACE(VulkanEngine)

void OnmiCamera::prepareUniformBuffers() {
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_uniformBuffer, sizeof(m_mvp), &m_mvp));
    VK_CHECK_RESULT(m_uniformBuffer.map());
    updateUniformBuffers();
}

void OnmiCamera::updateUniformBuffers() {
    m_mvp.projection = glm::perspective((float)(M_PI / 2.0), 1.0f, 0.1f, 64.0f);
    m_mvp.view = glm::mat4(1.0f);
    m_mvp.model = glm::translate(glm::mat4(1.0f), glm::vec3(-m_mvp.lightpos.x, -m_mvp.lightpos.y, -m_mvp.lightpos.z));
    memcpy(m_uniformBuffer.mapped, &m_mvp, sizeof(m_mvp));
}

END_NAMESPACE(VulkanEngine)