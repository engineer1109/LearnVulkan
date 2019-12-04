#include "vulkan_basicengine_vertexobject.h"
namespace VulkanTemplate {

VertexObject::~VertexObject(){
    if(m_pipeline){
        vkDestroyPipeline(m_device, m_pipeline, nullptr);
    }
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
    m_uniformBuffers.destroy();
}

void VertexObject::create(){
    generateVertex();
    prepareUniformBuffers();
}

void VertexObject::update(){

}

void VertexObject::generateVertex(){

}

void VertexObject::prepareUniformBuffers(){
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBuffers,
        sizeof(m_uboVS),
        &m_uboVS));

    updateUniformBuffers();
}

void VertexObject::updateUniformBuffers(){

}

}
