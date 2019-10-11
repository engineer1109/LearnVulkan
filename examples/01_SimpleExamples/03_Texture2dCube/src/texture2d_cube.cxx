#include "texture2d_cube.h"
#define VERTEX_BUFFER_BIND_ID 0
Texture2dCube::Texture2dCube(bool debugLayer):VulkanBasicEngine(debugLayer){
    this->zoom=-4.f;
    this->settings.overlay=true;
}
Texture2dCube::~Texture2dCube(){
    vkDestroyPipeline(device, m_pipeline, nullptr);
    vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
    m_vertexBuffer.destroy();
    m_indexBuffer.destroy();
    m_uniformBufferVS.destroy();
}

void Texture2dCube::prepare(){
    VulkanBasicEngine::prepare();
    generateVertex();
    loadTexture2D();
    setupVertexDescriptions();
    prepareUniformBuffers();
    setupDescriptorSetLayout();
    preparePipelines();
    setupDescriptorPool();
    setupDescriptorSet();
    buildCommandBuffers();
    this->prepared=true;
}

void Texture2dCube::render()
{
    if (!prepared)
        return;
    draw();

    if (frameCounter == 0)
    {
        vkDeviceWaitIdle(device);
    }
    if (!paused || camera.updated){
        updateUniformBuffers(camera.updated);
        startAutoRotation();
    }
}

void Texture2dCube::draw()
{
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void Texture2dCube::OnUpdateUIOverlay(vks::UIOverlay *overlay)
{
    if (overlay->header("Settings")) {
        if (overlay->button("Auto Rotation")) {
            m_autoRotation=!m_autoRotation;
        }
     }
}

void Texture2dCube::generateVertex(){
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
    VK_CHECK_RESULT(vulkanDevice->createBuffer(
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_vertexBuffer,
        vertices.size() * sizeof(Vertex),
        vertices.data()));
    // Index buffer
    VK_CHECK_RESULT(vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_indexBuffer,
        indices.size() * sizeof(uint32_t),
        indices.data()));
}

void Texture2dCube::loadTexture2D(){
    m_texture.loadFromFile("../data/textures/awesomeface.png",VK_FORMAT_R8G8B8A8_UNORM,vulkanDevice,queue);
}

void Texture2dCube::setupVertexDescriptions()
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

void Texture2dCube::prepareUniformBuffers()
{
    // Vertex shader uniform buffer block
    VK_CHECK_RESULT(vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBufferVS,
        sizeof(m_uboVS),
        &m_uboVS));
    VK_CHECK_RESULT(m_uniformBufferVS.map());
    updateUniformBuffers(true);
}

void Texture2dCube::updateUniformBuffers(bool viewchanged)
{
    if (viewchanged)
    {
        m_uboVS.projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.001f, 256.0f);
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, zoom));
        m_uboVS.model = viewMatrix * glm::translate(glm::mat4(1.0f), cameraPos);
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    memcpy(m_uniformBufferVS.mapped, &m_uboVS, sizeof(m_uboVS));
}

void Texture2dCube::setupDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings =
    {
        // Binding 0 : Vertex shader uniform buffer
        vks::initializers::descriptorSetLayoutBinding(
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            VK_SHADER_STAGE_VERTEX_BIT,
            0),
        // Binding 1 : Fragment shader image sampler
        vks::initializers::descriptorSetLayoutBinding(
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            VK_SHADER_STAGE_FRAGMENT_BIT,
            1)
    };

    VkDescriptorSetLayoutCreateInfo descriptorLayout =
        vks::initializers::descriptorSetLayoutCreateInfo(
            setLayoutBindings.data(),
            static_cast<uint32_t>(setLayoutBindings.size()));

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &m_descriptorSetLayout));

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo =
        vks::initializers::pipelineLayoutCreateInfo(
            &m_descriptorSetLayout,
            1);

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
}

void Texture2dCube::preparePipelines()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
        vks::initializers::pipelineInputAssemblyStateCreateInfo(
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            0,
            VK_FALSE);

    VkPipelineRasterizationStateCreateInfo rasterizationState =
        vks::initializers::pipelineRasterizationStateCreateInfo(
            VK_POLYGON_MODE_FILL,
            VK_CULL_MODE_NONE,
            VK_FRONT_FACE_COUNTER_CLOCKWISE,
            0);

    VkPipelineColorBlendAttachmentState blendAttachmentState =
        vks::initializers::pipelineColorBlendAttachmentState(
            0xf,
            VK_FALSE);

    VkPipelineColorBlendStateCreateInfo colorBlendState =
        vks::initializers::pipelineColorBlendStateCreateInfo(
            1,
            &blendAttachmentState);

    VkPipelineDepthStencilStateCreateInfo depthStencilState =
        vks::initializers::pipelineDepthStencilStateCreateInfo(
            VK_TRUE,
            VK_TRUE,
            VK_COMPARE_OP_LESS_OR_EQUAL);

    VkPipelineViewportStateCreateInfo viewportState =
        vks::initializers::pipelineViewportStateCreateInfo(5, 5, 0);

    VkPipelineMultisampleStateCreateInfo multisampleState =
        vks::initializers::pipelineMultisampleStateCreateInfo(
            VK_SAMPLE_COUNT_1_BIT,
            0);

    std::vector<VkDynamicState> dynamicStateEnables = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
    VkPipelineDynamicStateCreateInfo dynamicState =
        vks::initializers::pipelineDynamicStateCreateInfo(
            dynamicStateEnables.data(),
            static_cast<uint32_t>(dynamicStateEnables.size()),
            0);

    // Load shaders
    std::array<VkPipelineShaderStageCreateInfo,2> shaderStages;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo =
        vks::initializers::pipelineCreateInfo(
            m_pipelineLayout,
            renderPass,
            0);

    pipelineCreateInfo.pVertexInputState = &m_vertices.inputState;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineCreateInfo.pStages = shaderStages.data();

    //pipelineCreateInfo.subpass=1;
    shaderStages[0] = loadShader(getShaderPath()+"shaders/03_Texture2dCube/texture.so.vert", VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(getShaderPath()+"shaders/03_Texture2dCube/texture.so.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline));
}

void Texture2dCube::setupDescriptorPool()
{
    // Example uses one ubo and one image sampler
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
    };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
        vks::initializers::descriptorPoolCreateInfo(
            static_cast<uint32_t>(poolSizes.size()),
            poolSizes.data(),
            2);

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
}

void Texture2dCube::setupDescriptorSet()
{
    VkDescriptorSetAllocateInfo allocInfo =
        vks::initializers::descriptorSetAllocateInfo(
            descriptorPool,
            &m_descriptorSetLayout,
            1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &m_descriptorSet));

    std::vector<VkWriteDescriptorSet> writeDescriptorSets =
    {
        // Binding 0 : Vertex shader uniform buffer
        vks::initializers::writeDescriptorSet(
            m_descriptorSet,
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            0,
            &m_uniformBufferVS.descriptor),
        // Binding 1 : Fragment shader texture sampler
        //	Fragment shader: layout (binding = 1) uniform sampler2D samplerColor;
        vks::initializers::writeDescriptorSet(
            m_descriptorSet,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,		// The descriptor set will use a combined image sampler (sampler and image could be split)
            1,												// Shader binding point 1
            &m_texture.descriptor)
    };

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, NULL);
}

void Texture2dCube::buildCommandBuffers()
{
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    VkClearValue clearValues[2];
    clearValues[0].color = { { 0.1f, 0.2f, 0.3f, 0.0f } };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = width;
    renderPassBeginInfo.renderArea.extent.height = height;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    for (int32_t i = 0; i < drawCmdBuffers.size(); ++i)
    {
            // Set target frame buffer
        renderPassBeginInfo.framebuffer = frameBuffers[i];

        VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo));

        vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkDeviceSize offsets[1] = { 0 };
        vkCmdBindDescriptorSets(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, NULL);

        VkViewport viewports = { 0, 0, 1.f*width, 1.f*height, 0.0, 1.0f };
        VkRect2D scissorRects=vks::initializers::rect2D(width, height, 0, 0);

        vkCmdSetViewport(drawCmdBuffers[i], 0, 8, &viewports);
        vkCmdSetScissor(drawCmdBuffers[i], 0, 8, &scissorRects);

        vkCmdBindPipeline(drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
        vkCmdBindVertexBuffers(drawCmdBuffers[i], VERTEX_BUFFER_BIND_ID, 1,&m_vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(drawCmdBuffers[i], m_indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(drawCmdBuffers[i], m_indexCount, 1, 0, 0, 0);

        drawUI(drawCmdBuffers[i]);

        vkCmdEndRenderPass(drawCmdBuffers[i]);
        VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
    vkQueueWaitIdle(queue);
}

void Texture2dCube::startAutoRotation(){
    if(m_autoRotation){
        m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(0.01f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

