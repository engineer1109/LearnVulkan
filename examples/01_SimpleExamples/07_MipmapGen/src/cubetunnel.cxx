#include "cubetunnel.h"
#define VERTEX_BUFFER_BIND_ID 0
CubeTunnel::~CubeTunnel(){
    m_texture.destroy();
    vkDestroyPipeline(m_device, m_pipeline, nullptr);
    m_uniformBufferVS.destroy();
}

void CubeTunnel::create(){
    generateVertex();
    loadTextures();
    prepareUniformBuffers();
}

void CubeTunnel::build(VkCommandBuffer cmd,VkPipelineLayout pipelineLayout){
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descriptorSet, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(cmd, VERTEX_BUFFER_BIND_ID, 1,&m_vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 0);
}

void CubeTunnel::update(){
    updateUniformBuffers();
}

void CubeTunnel::generateVertex(){
    std::vector<Vertex> vertices =
    {

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 0.0f,-1.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 0.0f,-1.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 0.0f,-1.0f } },
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 0.0f,-1.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 0.0f,-1.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 0.0f,-1.0f } },

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },

       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, {-1.0f, 0.0f, 0.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } },

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },

       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f,-1.0f, 0.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } },
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f } },
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f,-1.0f, 0.0f } },
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
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_vertexBuffer,
        vertices.size() * sizeof(Vertex),
        vertices.data()));
    // Index buffer
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_indexBuffer,
        indices.size() * sizeof(uint32_t),
        indices.data()));
}

void CubeTunnel::loadTextures(){
    m_texture.loadFromFileAutoGenMipmap("../data/textures/container.png",VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue);
}

void CubeTunnel::prepareUniformBuffers()
{
    // Vertex shader uniform buffer block
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBufferVS,
        sizeof(uboVS),
        &m_uboVS));

    updateUniformBuffers();
}

void CubeTunnel::updateUniformBuffers(){
    m_uboVS.projection = m_camera->matrices.perspective;
    m_uboVS.view = m_camera->matrices.view;
    m_uboVS.model = glm::rotate(glm::mat4(1.0f), glm::radians(*m_timer * 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.viewPos = glm::vec4(m_camera->position, 0.0f) * glm::vec4(-1.0f);
    VK_CHECK_RESULT(m_uniformBufferVS.map());
    memcpy(m_uniformBufferVS.mapped, &m_uboVS, sizeof(uboVS));
    m_uniformBufferVS.unmap();
}
