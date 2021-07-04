//
// Created by wjl on 2021/6/20.
//

#include "AssimpObject.h"

#ifdef BUILD_ASSIMP

#include "VulkanModelV2.hpp"

#endif

BEGIN_NAMESPACE(VulkanEngine)

#ifdef BUILD_ASSIMP

AssimpObject::~AssimpObject() {
    if(m_model){
        m_model->destroy();
        delete_ptr(m_model);
    }
}

void AssimpObject::generateVertex() {
    m_model = new vks::Model();
    vks::VertexLayout layout = vks::VertexLayout({
                                                         vks::VERTEX_COMPONENT_POSITION,
                                                         vks::VERTEX_COMPONENT_UVVEC4,
                                                         vks::VERTEX_COMPONENT_NORMAL,
                                                 });
#if !defined(__ANDROID__)
    m_model->loadFromFile(m_modelPath, layout, 0.05f, m_context->vulkanDevice, m_context->queue, nullptr);
#else
    m_model->loadFromFile(m_modelPath, layout, 0.05f, m_context->vulkanDevice, m_context->queue, m_context->m_asset);
#endif
}

void AssimpObject::build(VkCommandBuffer &cmdBuffer, VulkanShader* vulkanShader) {
    VkDeviceSize offsets[1] = {0};
    if (vulkanShader->getPipeLine()) {
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanShader->getPipeLine());
    } else {
        std::cout << "Pipeline null,bind failure" << std::endl;
    }
    vkCmdBindVertexBuffers(cmdBuffer, VERTEX_BUFFER_BIND_ID, 1, &(m_model->vertices.buffer), offsets);
    vkCmdBindIndexBuffer(cmdBuffer, m_model->indices.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer, m_model->indexCount, 1, 0, 0, 0);
}
#else

AssimpObject::~AssimpObject() {

}

void AssimpObject::generateVertex() {}

void AssimpObject::build(VkCommandBuffer &cmdBuffer, VulkanShader* vulkanShader) {}

#endif

END_NAMESPACE(VulkanEngine)