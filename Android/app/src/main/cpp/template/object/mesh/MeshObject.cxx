//
// Created by wjl on 21-2-28.
//

#include "MeshObject.h"

BEGIN_NAMESPACE(VulkanEngine)

MeshObject::~MeshObject() {
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
}

void MeshObject::prepare() {
    generateVertex();
}

void MeshObject::update() {
    updateVertex();
}

void MeshObject::build(VkCommandBuffer &cmdBuffer, VulkanShader* vulkanShader) {
    VkDeviceSize offsets[1] = {0};
    if (vulkanShader->getPipeLine()) {
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanShader->getPipeLine());
    } else {
        std::cout << "Pipeline null,bind failure" << std::endl;
    }
    vkCmdBindVertexBuffers(cmdBuffer, VERTEX_BUFFER_BIND_ID, 1, &m_vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer, m_indexCount, 1, 0, 0, 0);
}

END_NAMESPACE(VulkanEngine)