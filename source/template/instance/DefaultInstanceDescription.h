//
// Created by wjl on 21-4-11.
//

#ifndef LEARNVULKAN_DEFAULTINSTANCEDESCRIPTION_H
#define LEARNVULKAN_DEFAULTINSTANCEDESCRIPTION_H

#include "VulkanVertexDescriptions.h"

BEGIN_NAMESPACE(VulkanEngine)

struct InstanceData {
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 color;
    float scale;
    float texIndex;
};

class DefaultInstanceDescription : public VulkanVertexDescriptions {
public:
    DefaultInstanceDescription() = default;

    ~DefaultInstanceDescription() = default;

    void GenerateInstanceDescriptions() {
        // Binding description
        m_inputBinding.resize(2);
        m_inputBinding[0] = vks::initializers::vertexInputBindingDescription(
                VERTEX_BUFFER_BIND_ID, sizeof(VertexTexVec4), VK_VERTEX_INPUT_RATE_VERTEX);
        m_inputBinding[1] = vks::initializers::vertexInputBindingDescription(
                INSTANCE_BUFFER_BIND_ID, sizeof(InstanceData), VK_VERTEX_INPUT_RATE_INSTANCE);



        // Attribute descriptions
        // Describes memory layout and shader positions
        m_inputAttributes.resize(8);
        // Location 0 : Position
        m_inputAttributes[0] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexTexVec4, pos));
        // Location 1 : Texture coordinates
        m_inputAttributes[1] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 1, VK_FORMAT_R32G32B32A32_SFLOAT,
                offsetof(VertexTexVec4, uv));
        // Location 2 : Vertex normal
        m_inputAttributes[2] = vks::initializers::vertexInputAttributeDescription(
                VERTEX_BUFFER_BIND_ID, 2, VK_FORMAT_R32G32B32_SFLOAT,
                offsetof(VertexTexVec4, normal));

        // Location 3 : Cube Position
        m_inputAttributes[3] =
                vks::initializers::vertexInputAttributeDescription(INSTANCE_BUFFER_BIND_ID, 3,
                                                                   VK_FORMAT_R32G32B32_SFLOAT, 0);
        // Location 4 : Cube Rotation
        m_inputAttributes[4] = vks::initializers::vertexInputAttributeDescription(
                INSTANCE_BUFFER_BIND_ID, 4, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3);
        // Location 5 : Cube Color
        m_inputAttributes[5] = vks::initializers::vertexInputAttributeDescription(
                INSTANCE_BUFFER_BIND_ID, 5, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 6);
        // Location 6 : Scale
        m_inputAttributes[6] = vks::initializers::vertexInputAttributeDescription(
                INSTANCE_BUFFER_BIND_ID, 6, VK_FORMAT_R32_SFLOAT, sizeof(float) * 9);
        // Location 7 : index
        m_inputAttributes[7] = vks::initializers::vertexInputAttributeDescription(
                INSTANCE_BUFFER_BIND_ID, 7, VK_FORMAT_R32_SFLOAT, sizeof(float) * 10);

        m_inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
        m_inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_inputBinding.size());
        m_inputState.pVertexBindingDescriptions = m_inputBinding.data();
        m_inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_inputAttributes.size());
        m_inputState.pVertexAttributeDescriptions = m_inputAttributes.data();
    }
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_DEFAULTINSTANCEDESCRIPTION_H
