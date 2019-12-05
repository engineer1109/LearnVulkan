/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "cubetunnel.h"
#include "mipmapgen.h"
#define VERTEX_BUFFER_BIND_ID 0
MipmapGen::MipmapGen(bool debug): VulkanBasicEngine (debug){
    title="Mipmap AutpGen";
    settings.overlay = true;

    camera.type = Camera::CameraType::firstperson;
    camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1024.0f);
    camera.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    camera.setTranslation(glm::vec3(0.f, 0.0f, -3.0f));
    camera.movementSpeed = 2.5f;
    camera.rotationSpeed = 0.5f;
    settings.overlay = true;
    timerSpeed *= 0.05f;
    paused = true;
}

MipmapGen::~MipmapGen(){

}

void MipmapGen::prepare(){
    VulkanBasicEngine::prepare();
    createObjects();
    setupVertexDescriptions();
    setupDescriptorPool();
    setupDescriptorSetLayout();
    setupDescriptorSet();
    preparePipelines();
    buildCommandBuffers();
    prepared=true;
}

void MipmapGen::render(){
    if (!prepared)
        return;
    draw();

    if (frameCounter == 0)
    {
        vkDeviceWaitIdle(device);
    }

    m_cubeTunnel->update();
}

void MipmapGen::draw(){
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void MipmapGen::OnUpdateUIOverlay(vks::UIOverlay *overlay){
    if (overlay->header("Settings")) {
        if (overlay->sliderFloat("LOD bias", &m_cubeTunnel->m_uboVS.lodBias, 0.0f, float(m_cubeTunnel->m_texture.mipLevels))) {
            m_cubeTunnel->update();
        }
        if (overlay->comboBox("Sampler type", &m_cubeTunnel->m_uboVS.samplerIndex, m_cubeTunnel->m_texture.samplerNames)) {
            m_cubeTunnel->update();
        }
    }
}

void MipmapGen::createObjects(){
    VulkanTemplate::VulkanBaseObject::ObjectInfo objectinfo;
    objectinfo.vulkanDevice=vulkanDevice;
    objectinfo.instance=instance;
    objectinfo.cmdPool=cmdPool;
    objectinfo.pipelineCache=pipelineCache;
    objectinfo.renderPass=renderPass;
    objectinfo.queue=queue;
    objectinfo.screenWitdh=&width;
    objectinfo.screenHeight=&height;

    m_cubeTunnel=new CubeTunnel();
    m_cubeTunnel->setCamera(&camera);
    m_cubeTunnel->setTimer(&timer);
    m_cubeTunnel->setObjectInfo(objectinfo);
    m_cubeTunnel->create();
}

void MipmapGen::setupVertexDescriptions()
{
    // Binding description
    m_vertices.bindingDescriptions.resize(1);
    m_vertices.bindingDescriptions[0] =
        vks::initializers::vertexInputBindingDescription(
            VERTEX_BUFFER_BIND_ID,
            sizeof(Vertex),
            VK_VERTEX_INPUT_RATE_VERTEX);

    // Attribute descriptions
    // Describes memory layout and shader positions
    m_vertices.attributeDescriptions.resize(3);
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
    // Location 1 : Vertex normal
    m_vertices.attributeDescriptions[2] =
        vks::initializers::vertexInputAttributeDescription(
            VERTEX_BUFFER_BIND_ID,
            2,
            VK_FORMAT_R32G32B32_SFLOAT,
            5 * sizeof(float));

    m_vertices.inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
    m_vertices.inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(m_vertices.bindingDescriptions.size());
    m_vertices.inputState.pVertexBindingDescriptions = m_vertices.bindingDescriptions.data();
    m_vertices.inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_vertices.attributeDescriptions.size());
    m_vertices.inputState.pVertexAttributeDescriptions = m_vertices.attributeDescriptions.data();
}

void MipmapGen::setupDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),	// Vertex shader UBO
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1),		// Sampled image
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 3),			// 3 samplers (array)
    };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
        vks::initializers::descriptorPoolCreateInfo(
            static_cast<uint32_t>(poolSizes.size()),
            poolSizes.data(),
            1);

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
}

void MipmapGen::setupDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;

    // Binding 0: Vertex shader uniform buffer
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_SHADER_STAGE_VERTEX_BIT,
        0));

    // Binding 1: Sampled image
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        1));

    // Binding 2: Sampler array (3 descriptors)
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_SAMPLER,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        2,
        3));

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

void MipmapGen::preparePipelines()
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
        vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);

    VkPipelineMultisampleStateCreateInfo multisampleState =
        vks::initializers::pipelineMultisampleStateCreateInfo(
            VK_SAMPLE_COUNT_1_BIT,
            0);

    std::vector<VkDynamicState> dynamicStateEnables = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH,
    };
    VkPipelineDynamicStateCreateInfo dynamicState =
        vks::initializers::pipelineDynamicStateCreateInfo(
            dynamicStateEnables.data(),
            static_cast<uint32_t>(dynamicStateEnables.size()),
            0);

    // Load shaders
    std::array<VkPipelineShaderStageCreateInfo,2> shaderStages;

    shaderStages[0] = loadShader(FS::getAssetPath("shaders/07_MipmapGen/texture.so.vert"), VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(FS::getAssetPath("shaders/07_MipmapGen/texture.so.frag"), VK_SHADER_STAGE_FRAGMENT_BIT);

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

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &(m_cubeTunnel->m_pipeline)));
}

void MipmapGen::setupDescriptorSet()
{
    VkDescriptorSetAllocateInfo allocInfo =
        vks::initializers::descriptorSetAllocateInfo(
            descriptorPool,
            &m_descriptorSetLayout,
            1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &m_cubeTunnel->m_descriptorSet));

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;

    // Binding 0: Vertex shader uniform buffer
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
        m_cubeTunnel->m_descriptorSet,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        0,
        &(m_cubeTunnel->m_uniformBufferVS).descriptor));

    // Binding 1: Sampled image
    VkDescriptorImageInfo textureDescriptor =
        vks::initializers::descriptorImageInfo(
            VK_NULL_HANDLE,
            m_cubeTunnel->m_texture.view,
            m_cubeTunnel->m_texture.imageLayout);
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
        m_cubeTunnel->m_descriptorSet,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        1,
        &textureDescriptor));

    // Binding 2: Sampler array
    std::vector<VkDescriptorImageInfo> samplerDescriptors;
    for (auto i = 0; i < m_cubeTunnel->m_texture.samplers.size(); i++)
    {
        samplerDescriptors.push_back(vks::initializers::descriptorImageInfo(m_cubeTunnel->m_texture.samplers[i], VK_NULL_HANDLE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
    }
    VkWriteDescriptorSet samplerDescriptorWrite{};
    samplerDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    samplerDescriptorWrite.dstSet = m_cubeTunnel->m_descriptorSet;
    samplerDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerDescriptorWrite.descriptorCount = static_cast<uint32_t>(samplerDescriptors.size());
    samplerDescriptorWrite.pImageInfo = samplerDescriptors.data();
    samplerDescriptorWrite.dstBinding = 2;
    samplerDescriptorWrite.dstArrayElement = 0;
    writeDescriptorSets.push_back(samplerDescriptorWrite);

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, NULL);
}

void MipmapGen::buildCommandBuffers(){
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    VkClearValue clearValues[2];
    clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
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

        VkViewport viewports = { 0, 0, 1.f*width, 1.f*height, 0.0, 1.0f };
        VkRect2D scissorRects=vks::initializers::rect2D(width, height, 0, 0);

        vkCmdSetViewport(drawCmdBuffers[i], 0, 8, &viewports);
        vkCmdSetScissor(drawCmdBuffers[i], 0, 8, &scissorRects);

        m_cubeTunnel->build(drawCmdBuffers[i],m_pipelineLayout);

        drawUI(drawCmdBuffers[i]);

        vkCmdEndRenderPass(drawCmdBuffers[i]);
        VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
    vkQueueWaitIdle(queue);
}
