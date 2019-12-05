/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "vulkan_basicengine_filesystem.h"
#include "vkCube.h"
#define VERTEX_BUFFER_BIND_ID 0
VkCube::VkCube(){

}
VkCube::~VkCube(){
    m_textureA.destroy();
    m_textureB.destroy();
    vkDestroyPipeline(m_device, m_pipeline, nullptr);
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
    m_uniformBufferVS.destroy();
}
void VkCube::setObjectInfo(ObjectInfo info){
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
    this->m_screenWidth=info.screenWitdh;
    this->m_screenHeight=info.screenHeight;
}

void VkCube::setCamera(ObjectCamera camera){
    m_camera=camera;
}

void VkCube::setLocation(float x,float y,float z){
    m_x=x;
    m_y=y;
    m_z=z;
}
void VkCube::setSize(float size){
    m_size=size;
}

void VkCube::create(){
    generateVertex();
    setupVertexDescriptions();
    loadTexture2D();
    prepareUniformBuffers();
}

void VkCube::build(VkCommandBuffer cmd){
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(cmd, VERTEX_BUFFER_BIND_ID, 1,&m_vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 0);
}

void VkCube::update(){
    updateUniformBuffers(true);
}

void VkCube::generateVertex(){
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
    m_textureA.loadFromFile(FS::getAssetPath("textures/awesomeface.png"),VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue);
    m_textureB.loadFromFile(FS::getAssetPath("textures/container.png"),VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue);
}

void VkCube::prepareUniformBuffers()
{
    // Vertex shader uniform buffer block
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBufferVS,
        sizeof(m_uboVS),
        &m_uboVS));
    VK_CHECK_RESULT(m_uniformBufferVS.map());
    updateUniformBuffers(true);
}
void VkCube::updateUniformBuffers(bool viewchanged)
{
    if (viewchanged)
    {
        m_uboVS.projection = glm::perspective(glm::radians(60.0f), float(*m_screenWidth) / float(*m_screenHeight), 0.001f, 256.0f);
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, *m_camera.zoom));
        m_uboVS.model = viewMatrix * glm::translate(glm::mat4(1.0f), *m_camera.cameraPos);
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_uboVS.viewPos = glm::vec4(0.0f, 0.0f, -*m_camera.zoom, 0.0f);
    }
    memcpy(m_uniformBufferVS.mapped, &m_uboVS, sizeof(m_uboVS));
}
