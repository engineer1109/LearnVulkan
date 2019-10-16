#include "vkCube.h"
VkCube::VkCube(){

}
VkCube::~VkCube(){

}
void VkCube::setDeviceInfo(VkObjectInfo info){
    this->m_vulkanDevice=info.vulkanDevice;
    this->m_device=this->m_vulkanDevice->logicalDevice;
    this->m_physicalDevice=this->m_vulkanDevice->physicalDevice;
    this->m_instance=info.instance;
    this->m_cmdPool=info.cmdPool;
    this->m_pipelineLayout=info.pipelineLayout;
    this->m_pipelineCache=info.pipelineCache;
    this->m_pipelineLayout=info.pipelineLayout;
    this->m_renderPass=info.renderPass;
}

void VkCube::create(){
    generateVertex();
}

void VkCube::generateVertex(){
    // Setup vertices for a single uv-mapped quad made from two triangles
    std::vector<Vertex> vertices =
    {

       { {  1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { { -1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { {  1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },

       { {  1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f,  1.0f, -1.0f },{ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { {  1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },

       { {  1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { {  1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { {  1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f,  1.0f, -1.0f },{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { {  1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },

       { { -1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
       { { -1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f, -1.0f, -1.0f },{ 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },

       { {  1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { { -1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f,  1.0f, -1.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { {  1.0f,  1.0f,  1.0f },{ 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f,  1.0f, -1.0f },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f,  1.0f, -1.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },

       { {  1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { { -1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
       { {  1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
       { {  1.0f, -1.0f, -1.0f },{ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
       { { -1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
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
