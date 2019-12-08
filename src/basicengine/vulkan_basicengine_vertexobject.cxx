/*
* LearnVulkan BasicEngine
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
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
}

void VertexObject::build(VkCommandBuffer cmd){
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pPipelineLayout, 0, 1, &m_descriptorSet, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(cmd, VERTEX_BUFFER_BIND_ID, 1,&m_vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 0);
}

void VertexObject::update(){

}

void VertexObject::generateVertex(){

}



}
