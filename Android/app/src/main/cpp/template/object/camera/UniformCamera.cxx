//
// Created by wjl on 21-2-28.
//

#include "UniformCamera.h"

BEGIN_NAMESPACE(VulkanEngine)

void UniformCamera::prepareUniformBuffers() {
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_uniformBuffer, sizeof(m_uboVS), &m_uboVS));
    VK_CHECK_RESULT(m_uniformBuffer.map());
    updateUniformBuffers();
}

void UniformCamera::updateUniformBuffers() {
    m_uboVS.projection =
            glm::perspective(glm::radians(60.0f),
                             static_cast<float>(*m_context->pScreenWidth) /
                             static_cast<float>(*m_context->pScreenHeight),
                             0.001f, 256.0f);
    m_uboVS.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, *m_pZoom));
    m_uboVS.model = glm::translate(glm::mat4(1.0f), *m_pCameraPos);;
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(-m_pRotation->x),
                                glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_pRotation->y),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_pRotation->z),
                                glm::vec3(0.0f, 0.0f, 1.0f));
    m_uboVS.normal = glm::inverseTranspose(m_uboVS.view * m_uboVS.model);
    //m_uboVS.lightpos = glm::vec4(0.f, 0.f, -4.f, 0.f);
    memcpy(m_uniformBuffer.mapped, &m_uboVS, sizeof(m_uboVS));
}

END_NAMESPACE(VulkanEngine)
