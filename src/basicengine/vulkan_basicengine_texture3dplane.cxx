#include "vulkan_basicengine_texture3dplane.h"
BEGIN_NAMESPACE(VulkanTemplate)
Texture3DPlane::~Texture3DPlane(){
    m_texture3d.destroy();
}

void Texture3DPlane::create(){
    generateVertex();
    setupVertexDescriptions();
}

void Texture3DPlane::loadTexture3D(uint8_t *imgData, uint32_t width, uint32_t height, uint32_t depth, VkFormat format){
    m_texture3d.loadFromArray(imgData,width,height,depth,format,m_vulkanDevice,m_queue);
}

void Texture3DPlane::generateVertex(){
    // Setup vertices for a single uv-mapped quad made from two triangles
    std::vector<Vertex> vertices =
    {
        { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
    };

    // Setup indices
    std::vector<uint32_t> indices = { 0,1,2, 2,3,0 };
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

void Texture3DPlane::setupVertexDescriptions()
{
    // Binding description
    m_vertices.inputBinding.resize(1);
    m_vertices.inputBinding[0] =
    vks::initializers::vertexInputBindingDescription(
        VERTEX_BUFFER_BIND_ID,
        sizeof(Vertex),
        VK_VERTEX_INPUT_RATE_VERTEX);
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
        VK_FORMAT_R32G32B32_SFLOAT,
        offsetof(Vertex, uvw));
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
END_NAMESPACE(VulkanTemplate)
