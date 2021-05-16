//
// Created by wjl on 21-5-17.
//

#include "VulkanPlane.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanPlane::~VulkanPlane() noexcept {}

void VulkanPlane::generateVertex() {
    std::vector<VertexTexVec4> vertices =
            {

                    {{m_a,  m_b,  0.0f},  {1.0f, 1.0f}, {0.0f,  0.0f,  1.0f}},
                    {{-m_a, -m_b, 0.0f},  {0.0f, 0.0f}, {0.0f,  0.0f,  1.0f}},
                    {{-m_a, m_b,  0.0f},  {0.0f, 1.0f}, {0.0f,  0.0f,  1.0f}},
                    {{m_a,  m_b,  0.0f},  {1.0f, 1.0f}, {0.0f,  0.0f,  1.0f}},
                    {{m_a,  -m_b, 0.0f},  {1.0f, 0.0f}, {0.0f,  0.0f,  1.0f}},
                    {{-m_a, -m_b, 0.0f},  {0.0f, 0.0f}, {0.0f,  0.0f,  1.0f}},

            };

    // Setup indices
    std::vector<uint32_t> indices(vertices.size());
    for(int i=0;i<indices.size();i++){
        indices[i]=i;
    }
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