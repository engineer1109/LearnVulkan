//
// Created by wjl on 21-2-28.
//

#include "Triangle.h"

BEGIN_NAMESPACE(VulkanEngine)

Triangle::~Triangle() {}

void Triangle::generateVertex() {
    std::vector<VertexTexVec4> vertices =
            {
                    {{0.0f,  -1.0f, 0.0f}, {1.0f, 0.0f, 0.f, 0.f}, {0.0f, 0.0f, 1.0f}},
                    {{1.0f,  1.0f,  0.0f}, {0.0f, 1.0f, 0.f, 0.f}, {0.0f, 0.0f, 1.0f}},
                    {{-1.0f, 1.0f,  0.0f}, {0.0f, 0.0f, 1.f, 0.f}, {0.0f, 0.0f, 1.0f}},
            };

    // Setup indices
    std::vector<uint32_t> indices = {0, 1, 2};
    m_indexCount = static_cast<uint32_t>(indices.size());

    // Create buffers
    // For the sake of simplicity we won't stage the vertex data to the gpu memory
    // Vertex buffer
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_vertexBuffer,
            vertices.size() * sizeof(VertexTexVec4),
            vertices.data()));
    // Index buffer
    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_indexBuffer,
            indices.size() * sizeof(uint32_t),
            indices.data()));
}

END_NAMESPACE(VulkanEngine)