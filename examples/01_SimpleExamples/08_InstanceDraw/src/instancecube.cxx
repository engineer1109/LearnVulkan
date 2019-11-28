#include "instancecube.h"
#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1
InstanceCube::~InstanceCube(){

}

void InstanceCube::create(){
    setupVertexDescriptions();
    generateVertex();
}

void InstanceCube::build(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout){
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_descriptorSet, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(cmd, VERTEX_BUFFER_BIND_ID, 1,&m_vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 0);
}

void InstanceCube::update(){

}

void InstanceCube::setupVertexDescriptions(){
    // Binding description
    m_vertices.bindingDescriptions.resize(2);
    m_vertices.bindingDescriptions[0] =
        vks::initializers::vertexInputBindingDescription(
            VERTEX_BUFFER_BIND_ID,
            sizeof(Vertex),
            VK_VERTEX_INPUT_RATE_VERTEX);
    m_vertices.bindingDescriptions[1] =
        vks::initializers::vertexInputBindingDescription(
            INSTANCE_BUFFER_BIND_ID,
            sizeof(InstanceData),
            VK_VERTEX_INPUT_RATE_INSTANCE);

    // Attribute descriptions
    // Describes memory layout and shader positions
    m_vertices.attributeDescriptions.resize(8);
    // Location 0 : Position
    m_vertices.attributeDescriptions[0] =
        vks::initializers::vertexInputAttributeDescription(
            VERTEX_BUFFER_BIND_ID,
            0,
            VK_FORMAT_R32G32B32_SFLOAT,
            0);
    // Location 1 : Texture coordinates
    m_vertices.attributeDescriptions[1] =
        vks::initializers::vertexInputAttributeDescription(
            VERTEX_BUFFER_BIND_ID,
            1,
            VK_FORMAT_R32G32_SFLOAT,
            3 * sizeof(float));
    // Location 2 : Vertex normal
    m_vertices.attributeDescriptions[2] =
        vks::initializers::vertexInputAttributeDescription(
            VERTEX_BUFFER_BIND_ID,
            2,
            VK_FORMAT_R32G32B32_SFLOAT,
            5 * sizeof(float));
    // Location 3 : Color
    m_vertices.attributeDescriptions[3] =
        vks::initializers::vertexInputAttributeDescription(
            VERTEX_BUFFER_BIND_ID,
            3,
            VK_FORMAT_R32G32B32_SFLOAT,
            8 * sizeof(float));

    // Location 4 : Cube Position
    m_vertices.attributeDescriptions[4] =
        vks::initializers::vertexInputAttributeDescription(
            INSTANCE_BUFFER_BIND_ID,
            4,
            VK_FORMAT_R32G32B32_SFLOAT,
            0);
    // Location 5 : Cube Rotation
    m_vertices.attributeDescriptions[5] =
        vks::initializers::vertexInputAttributeDescription(
            INSTANCE_BUFFER_BIND_ID,
            5,
            VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(float) * 3);
    // Location 6 : Scale
    m_vertices.attributeDescriptions[6] =
        vks::initializers::vertexInputAttributeDescription(
            INSTANCE_BUFFER_BIND_ID,
            6,
            VK_FORMAT_R32_SFLOAT,
            sizeof(float) * 6);
    // Location 7 : index
    m_vertices.attributeDescriptions[7] =
        vks::initializers::vertexInputAttributeDescription(
            INSTANCE_BUFFER_BIND_ID,
            7,
            VK_FORMAT_R32_SFLOAT,
            sizeof(float) * 7);

    m_vertices.inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
    m_vertices.inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_vertices.bindingDescriptions.size());
    m_vertices.inputState.pVertexBindingDescriptions = m_vertices.bindingDescriptions.data();
    m_vertices.inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_vertices.attributeDescriptions.size());
    m_vertices.inputState.pVertexAttributeDescriptions = m_vertices.attributeDescriptions.data();
}

void InstanceCube::generateVertex(){
    std::vector<Vertex> vertices =
    {

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } ,{0.0f,0.0f,0.0f}},

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 0.0f,-1.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 0.0f,-1.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 0.0f,-1.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 0.0f,-1.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 0.0f,-1.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 0.0f,-1.0f } ,{0.0f,0.0f,0.0f}},

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},

       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, {-1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } ,{0.0f,0.0f,0.0f}},

       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x,  1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},

       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f,-1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 0.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y,  1.0f*m_size+m_z },{ 1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { {  1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
       { { -1.0f*m_size+m_x, -1.0f*m_size+m_y, -1.0f*m_size+m_z },{ 0.0f, 0.0f }, { 0.0f,-1.0f, 0.0f } ,{0.0f,0.0f,0.0f}},
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
