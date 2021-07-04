//
// Created by wjl on 21-4-10.
//

#include "ReflectParaBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

BEGIN_NAMESPACE(AssimpSample)

void ReflectParaBuffer::reflect() {
    if(getReflect() <= 0.f){
        setReflect(1.f);
    }
    else{
        setReflect(0.f);
    }
}

void ReflectParaBuffer::prepareUniformBuffers() {
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_uniformBuffer, sizeof(m_para), &m_para));
    VK_CHECK_RESULT(m_uniformBuffer.map());
    updateUniformBuffers();
}

void ReflectParaBuffer::updateUniformBuffers() {
    memcpy(m_uniformBuffer.mapped, &m_para, sizeof(m_para));
}

END_NAMESPACE(AssimpSample)

END_NAMESPACE(VulkanEngine)