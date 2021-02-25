//
// Created by wjl on 21-2-25.
//

#ifndef LEARNVULKAN_VULKANVERTEXDESCRIPTIONS_H
#define LEARNVULKAN_VULKANVERTEXDESCRIPTIONS_H

#include "vertex_struct.h"
#include "render_common.h"

#include "VulkanInitializers.hpp"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanVertexDescriptions {
public:
    VulkanVertexDescriptions() = default;
    ~VulkanVertexDescriptions() = default;

    VkPipelineVertexInputStateCreateInfo m_inputState;
    std::vector<VkVertexInputBindingDescription> m_inputBinding;
    std::vector<VkVertexInputAttributeDescription> m_inputAttributes;

    void GenerateUVDescriptions() {
        // Binding description
        m_inputBinding.resize(1);
        m_inputBinding[0] = vks::initializers::vertexInputBindingDescription(VERTEX_BUFFER_BIND_ID, sizeof(Vertex),
                                                                                 VK_VERTEX_INPUT_RATE_VERTEX);

        // Attribute descriptions
        // Describes memory layout and shader positions
        m_inputAttributes.resize(3);
        // Location 0 : Position
        m_inputAttributes[0] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos));
        // Location 1 : Texture coordinates
        m_inputAttributes[1] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv));
        // Location 1 : Vertex normal
        m_inputAttributes[2] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal));

        m_inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
        m_inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_inputBinding.size());
        m_inputState.pVertexBindingDescriptions = m_inputBinding.data();
        m_inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_inputAttributes.size());
        m_inputState.pVertexAttributeDescriptions = m_inputAttributes.data();
    }

    void GenerateUVWDescriptions() {
        // Binding description
        m_inputBinding.resize(1);
        m_inputBinding[0] = vks::initializers::vertexInputBindingDescription(
                VERTEX_BUFFER_BIND_ID, sizeof(VertexUVW), VK_VERTEX_INPUT_RATE_VERTEX);
        //    vertices.inputBinding[1] =
        //    vks::initializers::vertexInputBindingDescription(
        //        VERTEX_BUFFER_BIND_ID,
        //        sizeof(Vertex),
        //        VK_VERTEX_INPUT_RATE_VERTEX);

        // Attribute descriptions
        // Describes memory layout and shader positions
        m_inputAttributes.resize(3);
        // Location 0 : Position
        m_inputAttributes[0] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexUVW, pos));
        // Location 1 : Texture coordinates
        m_inputAttributes[1] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexUVW, uv));
        // Location 1 : Vertex normal
        m_inputAttributes[2] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexUVW, normal));

        m_inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
        m_inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_inputBinding.size());
        m_inputState.pVertexBindingDescriptions = m_inputBinding.data();
        m_inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_inputAttributes.size());
        m_inputState.pVertexAttributeDescriptions = m_inputAttributes.data();
    }

    void GenerateTexVec4Descriptions() {
        // Binding description
        m_inputBinding.resize(1);
        m_inputBinding[0] = vks::initializers::vertexInputBindingDescription(
                VERTEX_BUFFER_BIND_ID, sizeof(VertexUVW), VK_VERTEX_INPUT_RATE_VERTEX);
        //    vertices.inputBinding[1] =
        //    vks::initializers::vertexInputBindingDescription(
        //        VERTEX_BUFFER_BIND_ID,
        //        sizeof(Vertex),
        //        VK_VERTEX_INPUT_RATE_VERTEX);

        // Attribute descriptions
        // Describes memory layout and shader positions
        m_inputAttributes.resize(3);
        // Location 0 : Position
        m_inputAttributes[0] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexTexVec4, pos));
        // Location 1 : Texture coordinates
        m_inputAttributes[1] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(VertexTexVec4, uv));
        // Location 1 : Vertex normal
        m_inputAttributes[2] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexTexVec4, normal));

        m_inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
        m_inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_inputBinding.size());
        m_inputState.pVertexBindingDescriptions = m_inputBinding.data();
        m_inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_inputAttributes.size());
        m_inputState.pVertexAttributeDescriptions = m_inputAttributes.data();
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANVERTEXDESCRIPTIONS_H
