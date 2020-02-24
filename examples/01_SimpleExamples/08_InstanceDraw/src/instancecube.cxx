/*
 * LearnVulkan Examples
 *
 * Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "instancecube.h"
#include "vulkan_basicengine_filesystem.h"
#include "vulkanexamplebase.h"
#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1
InstanceCube::~InstanceCube() {}

void InstanceCube::create() {
    setupVertexDescriptions();
    generateVertex();
    prepareInstanceData();
    loadTextures();
    prepareUniformBuffer();
    preparePipelines();
}

void InstanceCube::build(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout) {
    VkDeviceSize offsets[1] = {0};
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descriptorSet, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    // Binding point 0 : Mesh vertex buffer
    vkCmdBindVertexBuffers(cmd, VERTEX_BUFFER_BIND_ID, 1, &m_vertexBuffer.buffer, offsets);
    // Binding point 1 : Instance data buffer
    vkCmdBindVertexBuffers(cmd, INSTANCE_BUFFER_BIND_ID, 1, &m_instanceBuffer.buffer, offsets);

    vkCmdBindIndexBuffer(cmd, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

    // Render instances
    vkCmdDrawIndexed(cmd, m_indexCount, INSTANCE_COUNT, 0, 0, 0);
}

void InstanceCube::update() { updateUniformBuffers(); }

void InstanceCube::setupVertexDescriptions() {
    // Binding description
    m_vertices.bindingDescriptions.resize(2);
    m_vertices.bindingDescriptions[0] = vks::initializers::vertexInputBindingDescription(
        VERTEX_BUFFER_BIND_ID, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX);
    m_vertices.bindingDescriptions[1] = vks::initializers::vertexInputBindingDescription(
        INSTANCE_BUFFER_BIND_ID, sizeof(InstanceData), VK_VERTEX_INPUT_RATE_INSTANCE);

    // Attribute descriptions
    // Describes memory layout and shader positions
    m_vertices.attributeDescriptions.resize(8);
    // Location 0 : Position
    m_vertices.attributeDescriptions[0] =
        vks::initializers::vertexInputAttributeDescription(VERTEX_BUFFER_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);
    // Location 1 : Texture coordinates
    m_vertices.attributeDescriptions[1] = vks::initializers::vertexInputAttributeDescription(
        VERTEX_BUFFER_BIND_ID, 1, VK_FORMAT_R32G32_SFLOAT, 3 * sizeof(float));
    // Location 2 : Vertex normal
    m_vertices.attributeDescriptions[2] = vks::initializers::vertexInputAttributeDescription(
        VERTEX_BUFFER_BIND_ID, 2, VK_FORMAT_R32G32B32_SFLOAT, 5 * sizeof(float));
    // Location 3 : Color
    m_vertices.attributeDescriptions[3] = vks::initializers::vertexInputAttributeDescription(
        VERTEX_BUFFER_BIND_ID, 3, VK_FORMAT_R32G32B32_SFLOAT, 8 * sizeof(float));

    // Location 4 : Cube Position
    m_vertices.attributeDescriptions[4] =
        vks::initializers::vertexInputAttributeDescription(INSTANCE_BUFFER_BIND_ID, 4, VK_FORMAT_R32G32B32_SFLOAT, 0);
    // Location 5 : Cube Rotation
    m_vertices.attributeDescriptions[5] = vks::initializers::vertexInputAttributeDescription(
        INSTANCE_BUFFER_BIND_ID, 5, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3);
    // Location 6 : Scale
    m_vertices.attributeDescriptions[6] = vks::initializers::vertexInputAttributeDescription(
        INSTANCE_BUFFER_BIND_ID, 6, VK_FORMAT_R32_SFLOAT, sizeof(float) * 6);
    // Location 7 : index
    m_vertices.attributeDescriptions[7] = vks::initializers::vertexInputAttributeDescription(
        INSTANCE_BUFFER_BIND_ID, 7, VK_FORMAT_R32_SFLOAT, sizeof(float) * 7);

    m_vertices.inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
    m_vertices.inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_vertices.bindingDescriptions.size());
    m_vertices.inputState.pVertexBindingDescriptions = m_vertices.bindingDescriptions.data();
    m_vertices.inputState.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(m_vertices.attributeDescriptions.size());
    m_vertices.inputState.pVertexAttributeDescriptions = m_vertices.attributeDescriptions.data();
}

void InstanceCube::generateVertex() {
    std::vector<Vertex> vertices = {

        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 0.0f}},

        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {0.0f, 0.0f, -1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {0.0f, 0.0f, -1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {0.0f, 0.0f, -1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {0.0f, 0.0f, -1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {0.0f, 0.0f, -1.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {0.0f, 0.0f, -1.0f},
         {0.0f, 0.0f, 0.0f}},

        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},

        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {-1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {-1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {-1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {-1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {-1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {-1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},

        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, 1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},

        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {0.0f, -1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {0.0f, -1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {0.0f, 1.0f},
         {0.0f, -1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, 1.0f * m_size + m_z},
         {1.0f, 1.0f},
         {0.0f, -1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {1.0f, 0.0f},
         {0.0f, -1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {{-1.0f * m_size + m_x, -1.0f * m_size + m_y, -1.0f * m_size + m_z},
         {0.0f, 0.0f},
         {0.0f, -1.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
    };

    // Setup indices
    std::vector<uint32_t> indices(vertices.size());
    for (int i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }
    m_indexCount = static_cast<uint32_t>(indices.size());

    // Create buffers
    // For the sake of simplicity we won't stage the vertex data to the gpu memory
    // Vertex buffer
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_vertexBuffer, vertices.size() * sizeof(Vertex), vertices.data()));
    // Index buffer
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_indexBuffer, indices.size() * sizeof(uint32_t), indices.data()));
}

void InstanceCube::prepareInstanceData() {
    std::vector<InstanceData> instanceData;
    instanceData.resize(INSTANCE_COUNT);

    std::default_random_engine rndGenerator(0 ? 0 : (unsigned)time(nullptr));
    std::uniform_real_distribution<float> uniformDist(0.0, 1.0);
    std::uniform_int_distribution<uint32_t> rndTextureIndex(0, 1); // not use here

    // Distribute rocks randomly on two different rings
    for (auto i = 0; i < INSTANCE_COUNT / 2; i++) {
        glm::vec2 ring0{7.0f, 11.0f};
        glm::vec2 ring1{14.0f, 18.0f};

        float rho, theta;

        // Inner ring
        rho = sqrt((pow(ring0[1], 2.0f) - pow(ring0[0], 2.0f)) * uniformDist(rndGenerator) + pow(ring0[0], 2.0f));
        theta = 2.0 * M_PI * uniformDist(rndGenerator);
        instanceData[i].pos = glm::vec3(rho * cos(theta), uniformDist(rndGenerator) * 0.5f - 0.25f, rho * sin(theta));
        instanceData[i].rot = glm::vec3(M_PI * uniformDist(rndGenerator), M_PI * uniformDist(rndGenerator),
                                        M_PI * uniformDist(rndGenerator));
        instanceData[i].scale = 1.5f + uniformDist(rndGenerator) - uniformDist(rndGenerator);
        instanceData[i].texIndex = rndTextureIndex(rndGenerator);
        instanceData[i].scale *= 0.75f;

        // Outer ring
        rho = sqrt((pow(ring1[1], 2.0f) - pow(ring1[0], 2.0f)) * uniformDist(rndGenerator) + pow(ring1[0], 2.0f));
        theta = 2.0 * M_PI * uniformDist(rndGenerator);
        instanceData[i + INSTANCE_COUNT / 2].pos =
            glm::vec3(rho * cos(theta), uniformDist(rndGenerator) * 0.5f - 0.25f, rho * sin(theta));
        instanceData[i + INSTANCE_COUNT / 2].rot = glm::vec3(
            M_PI * uniformDist(rndGenerator), M_PI * uniformDist(rndGenerator), M_PI * uniformDist(rndGenerator));
        instanceData[i + INSTANCE_COUNT / 2].scale = 1.5f + uniformDist(rndGenerator) - uniformDist(rndGenerator);
        instanceData[i + INSTANCE_COUNT / 2].texIndex = rndTextureIndex(rndGenerator);
        instanceData[i + INSTANCE_COUNT / 2].scale *= 0.75f;
    }

    m_instanceBuffer.size = instanceData.size() * sizeof(InstanceData);

    // Staging
    // Instanced data is static, copy to device local memory
    // This results in better performance

    struct {
        VkDeviceMemory memory;
        VkBuffer buffer;
    } stagingBuffer;

    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_instanceBuffer.size, &stagingBuffer.buffer, &stagingBuffer.memory, instanceData.data()));

    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_instanceBuffer.size,
                                                 &m_instanceBuffer.buffer, &m_instanceBuffer.memory));

    // Copy to staging buffer
    VkCommandBuffer copyCmd = m_vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

    VkBufferCopy copyRegion = {};
    copyRegion.size = m_instanceBuffer.size;
    vkCmdCopyBuffer(copyCmd, stagingBuffer.buffer, m_instanceBuffer.buffer, 1, &copyRegion);

    m_vulkanDevice->flushCommandBuffer(copyCmd, m_queue, true);

    m_instanceBuffer.descriptor.range = m_instanceBuffer.size;
    m_instanceBuffer.descriptor.buffer = m_instanceBuffer.buffer;
    m_instanceBuffer.descriptor.offset = 0;

    // Destroy staging resources
    vkDestroyBuffer(m_device, stagingBuffer.buffer, nullptr);
    vkFreeMemory(m_device, stagingBuffer.memory, nullptr);
}

void InstanceCube::loadTextures() {
    m_texture.loadFromFileAutoGenMipmap(FS::getAssetPath("textures/container.png"), VK_FORMAT_R8G8B8A8_UNORM,
                                        m_vulkanDevice, m_queue);
}

void InstanceCube::prepareUniformBuffer() {
    // Vertex shader uniform buffer block
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBufferVS, sizeof(m_uboVS), &m_uboVS));
    VK_CHECK_RESULT(m_uniformBufferVS.map());
    updateUniformBuffers();
}

void InstanceCube::updateUniformBuffers() {
    m_uboVS.projection =
        glm::perspective(glm::radians(60.0f), float(*m_screenWidth) / float(*m_screenHeight), 0.001f, 256.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, *m_camera.zoom));
    m_uboVS.model = viewMatrix * glm::translate(glm::mat4(1.0f), *m_camera.cameraPos);
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->z), glm::vec3(0.0f, 0.0f, 1.0f));
    // m_uboVS.viewPos = glm::vec4(0.0f, 0.0f, -*m_camera.zoom, 0.0f);
    memcpy(m_uniformBufferVS.mapped, &m_uboVS, sizeof(m_uboVS));
}

void InstanceCube::preparePipelines() {}
