//
// Created by wjl on 21-4-15.
//

#include "InstanceCube.h"

BEGIN_NAMESPACE(VulkanEngine)

InstanceCube::~InstanceCube() {}

void InstanceCube::prepare() {
    generateVertex();
    createInstanceData();
}

void InstanceCube::createInstanceData() {
    std::vector<InstanceData> instanceData;
    instanceData.resize(m_instanceCount);

    std::default_random_engine rndGenerator(0 ? 0 : (unsigned)time(nullptr));
    std::uniform_real_distribution<float> uniformDist(0.0, 1.0);
    std::uniform_int_distribution<uint32_t> rndTextureIndex(0, 1); // not use here

    // Distribute rocks randomly on two different rings
    for (auto i = 0; i < m_instanceCount / 2; i++) {
        glm::vec2 ring0{7.0f, 11.0f};
        glm::vec2 ring1{14.0f, 18.0f};

        float rho, theta;

        // Inner ring
        rho = sqrt((pow(ring0[1], 2.0f) - pow(ring0[0], 2.0f)) * uniformDist(rndGenerator) + pow(ring0[0], 2.0f));
        theta = 2.0 * M_PI * uniformDist(rndGenerator);
        instanceData[i].pos = glm::vec3(rho * cos(theta), uniformDist(rndGenerator) * 0.5f - 0.25f, rho * sin(theta));
        instanceData[i].rot = glm::vec3(M_PI * uniformDist(rndGenerator), M_PI * uniformDist(rndGenerator),
                                        M_PI * uniformDist(rndGenerator));
        instanceData[i].color = glm::vec3(uniformDist(rndGenerator), uniformDist(rndGenerator), uniformDist(rndGenerator));
        instanceData[i].scale = 1.5f + uniformDist(rndGenerator) - uniformDist(rndGenerator);
        instanceData[i].texIndex = rndTextureIndex(rndGenerator);
        instanceData[i].scale *= 0.75f;

        // Outer ring
        rho = sqrt((pow(ring1[1], 2.0f) - pow(ring1[0], 2.0f)) * uniformDist(rndGenerator) + pow(ring1[0], 2.0f));
        theta = 2.0 * M_PI * uniformDist(rndGenerator);
        instanceData[i + m_instanceCount / 2].pos =
                glm::vec3(rho * cos(theta), uniformDist(rndGenerator) * 0.5f - 0.25f, rho * sin(theta));
        instanceData[i + m_instanceCount / 2].rot = glm::vec3(
                M_PI * uniformDist(rndGenerator), M_PI * uniformDist(rndGenerator), M_PI * uniformDist(rndGenerator));
        instanceData[i + m_instanceCount / 2].color = glm::vec3(uniformDist(rndGenerator), uniformDist(rndGenerator), uniformDist(rndGenerator));
        instanceData[i + m_instanceCount / 2].scale = 1.5f + uniformDist(rndGenerator) - uniformDist(rndGenerator);
        instanceData[i + m_instanceCount / 2].texIndex = rndTextureIndex(rndGenerator);
        instanceData[i + m_instanceCount / 2].scale *= 0.75f;
    }

    createInstanceBuffer<InstanceData>(instanceData);
}

template<class T>
void InstanceCube::createInstanceBuffer(const std::vector<T> &instanceData) {
    struct {
        VkDeviceMemory memory;
        VkBuffer buffer;
    } stagingBuffer;

    m_instanceBuffer.size = instanceData.size() * sizeof(T);

    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_instanceBuffer.size, &stagingBuffer.buffer, &stagingBuffer.memory, (void*)instanceData.data()));

    VK_CHECK_RESULT(m_context->vulkanDevice->createBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_instanceBuffer.size,
                                                          &m_instanceBuffer.buffer, &m_instanceBuffer.memory));

    // Copy to staging buffer
    VkCommandBuffer copyCmd = m_context->vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

    VkBufferCopy copyRegion = {};
    copyRegion.size = m_instanceBuffer.size;
    vkCmdCopyBuffer(copyCmd, stagingBuffer.buffer, m_instanceBuffer.buffer, 1, &copyRegion);

    m_context->vulkanDevice->flushCommandBuffer(copyCmd, m_context->queue, true);

    m_instanceBuffer.descriptor.range = m_instanceBuffer.size;
    m_instanceBuffer.descriptor.buffer = m_instanceBuffer.buffer;
    m_instanceBuffer.descriptor.offset = 0;

    // Destroy staging resources
    vkDestroyBuffer(m_context->getDevice(), stagingBuffer.buffer, nullptr);
    vkFreeMemory(m_context->getDevice(), stagingBuffer.memory, nullptr);
}

void InstanceCube::build(VkCommandBuffer &cmdBuffer, VulkanShader* vulkanShader) {
    VkDeviceSize offsets[1] = {0};
    if (vulkanShader->getPipeLine()) {
        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanShader->getPipeLine());
    } else {
        std::cout << "Pipeline null,bind failure" << std::endl;
    }
    vkCmdBindVertexBuffers(cmdBuffer, VERTEX_BUFFER_BIND_ID, 1, &m_vertexBuffer.buffer, offsets);
    vkCmdBindVertexBuffers(cmdBuffer, INSTANCE_BUFFER_BIND_ID, 1, &m_instanceBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmdBuffer, m_indexCount, m_instanceCount, 0, 0, 0);
}

END_NAMESPACE(VulkanEngine)