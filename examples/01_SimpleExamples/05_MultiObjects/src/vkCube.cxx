#include "vkCube.h"
#define VERTEX_BUFFER_BIND_ID 0
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
    this->m_queue=info.queue;
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

void VkCube::setupVertexDescriptions()
{
    // Binding description
    m_vertices.inputBinding.resize(1);
    m_vertices.inputBinding[0] =
    vks::initializers::vertexInputBindingDescription(
        VERTEX_BUFFER_BIND_ID,
        sizeof(Vertex),
        VK_VERTEX_INPUT_RATE_VERTEX);
//    vertices.inputBinding[1] =
//    vks::initializers::vertexInputBindingDescription(
//        VERTEX_BUFFER_BIND_ID,
//        sizeof(Vertex),
//        VK_VERTEX_INPUT_RATE_VERTEX);

    // Attribute descriptions
    // Describes memory layout and shader positions
    m_vertices.inputAttributes.resize(3);
    // Location 0 : Position
    m_vertices.inputAttributes[0] =
    vks::initializers::vertexInputAttributeDescription(
        VERTEX_BUFFER_BIND_ID,
        0,
        VK_FORMAT_R32G32B32_SFLOAT,
        offsetof(Vertex, pos));
    // Location 1 : Texture coordinates
    m_vertices.inputAttributes[1] =
    vks::initializers::vertexInputAttributeDescription(
        VERTEX_BUFFER_BIND_ID,
        1,
        VK_FORMAT_R32G32_SFLOAT,
        offsetof(Vertex, uv));
    // Location 1 : Vertex normal
    m_vertices.inputAttributes[2] =
    vks::initializers::vertexInputAttributeDescription(
        VERTEX_BUFFER_BIND_ID,
        2,
        VK_FORMAT_R32G32B32_SFLOAT,
        offsetof(Vertex, normal));

    m_vertices.inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
    m_vertices.inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_vertices.inputBinding.size());
    m_vertices.inputState.pVertexBindingDescriptions = m_vertices.inputBinding.data();
    m_vertices.inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_vertices.inputAttributes.size());
    m_vertices.inputState.pVertexAttributeDescriptions = m_vertices.inputAttributes.data();
}

void VkCube::loadTexture2D(){
    m_textureA.loadFromFile("../data/textures/awesomeface.png",VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue);
    m_textureB.loadFromFile("../data/textures/container.png",VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue);
}
