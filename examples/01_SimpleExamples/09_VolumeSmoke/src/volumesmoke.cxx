/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "texture3dsmoke.h"
#include "volumesmoke.h"
VolumeSmoke::VolumeSmoke(bool debug):VulkanBasicEngine (debug){
    title="VolumeSmoke";
    settings.overlay = true;
}

VolumeSmoke::~VolumeSmoke(){
    if(m_volumeSmoke!=nullptr){
        delete m_volumeSmoke;
        m_volumeSmoke=nullptr;
    }
}

void VolumeSmoke::prepare(){
    VulkanBasicEngine::prepare();
    createObjects();
    setupDescriptorPool();
    setupDescriptorSetLayout();
    setupDescriptorSet();
    preparePipelines();
    buildCommandBuffers();
    prepared=true;
}

void VolumeSmoke::render(){
    if (!prepared)
        return;
    draw();

    if (frameCounter == 0)
    {
        vkDeviceWaitIdle(device);
    }
}

void VolumeSmoke::draw(){
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void VolumeSmoke::OnUpdateUIOverlay(vks::UIOverlay *overlay){

}

void VolumeSmoke::getEnabledFeatures()
{
    // Enable anisotropic filtering if supported
    if (deviceFeatures.samplerAnisotropy) {
        enabledFeatures.samplerAnisotropy = VK_TRUE;
    }
}

void VolumeSmoke::createObjects(){
    VulkanTemplate::VulkanBaseObject::ObjectInfo objectinfo;
    objectinfo.vulkanDevice=vulkanDevice;
    objectinfo.instance=instance;
    objectinfo.cmdPool=cmdPool;
    objectinfo.pPipelineLayout=&m_pipelineLayout;
    objectinfo.pPipelineCache=&pipelineCache;
    objectinfo.pRenderPass=&renderPass;
    objectinfo.queue=queue;
    objectinfo.screenWitdh=&width;
    objectinfo.screenHeight=&height;

    m_volumeSmoke=new Texture3dSmoke(128,128,128);
    m_volumeSmoke->setObjectInfo(objectinfo);
    float m_ModelView[16]=
    {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,4.0f,1.0f,
    };

    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            m_volumeSmoke->m_viewMat[i][j]=m_ModelView[4*i+j];
        }
    }

    m_volumeSmoke->create();
}

void VolumeSmoke::setupDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),	// Vertex shader UBO
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2),		// Sampled image
    };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
        vks::initializers::descriptorPoolCreateInfo(
            static_cast<uint32_t>(poolSizes.size()),
            poolSizes.data(),
            1);

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
}

void VolumeSmoke::setupDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;

    // Binding 0: Vertex shader uniform buffer
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_SHADER_STAGE_VERTEX_BIT,
        0));
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_SHADER_STAGE_VERTEX_BIT,
        1));

    // Binding 1: Sampled image
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        2));
    setLayoutBindings.push_back(vks::initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_SHADER_STAGE_FRAGMENT_BIT,
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

void VolumeSmoke::setupDescriptorSet()
{
    VkDescriptorSetAllocateInfo allocInfo =
        vks::initializers::descriptorSetAllocateInfo(
            descriptorPool,
            &m_descriptorSetLayout,
            1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &m_volumeSmoke->m_descriptorSet));

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;

    // Binding 0: Vertex shader uniform buffer
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
                                      m_volumeSmoke->m_descriptorSet,
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      0,
                                      &(m_volumeSmoke->m_uniformBuffers).descriptor));
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
                                      m_volumeSmoke->m_descriptorSet,
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      1,
                                      &(m_volumeSmoke->m_modelVolumeBuffer).descriptor));

    // Binding 1: Sampled image
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
                                      m_volumeSmoke->m_descriptorSet,
                                      VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                                      2,
                                      &(m_volumeSmoke->m_texture3d).descriptor));
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
                                      m_volumeSmoke->m_descriptorSet,
                                      VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                                      3,
                                      &(m_volumeSmoke->m_texture2d).descriptor));

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, NULL);
}

void VolumeSmoke::preparePipelines(){
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

    pipelineCreateInfo.pVertexInputState = &m_volumeSmoke->m_vertices.inputState;
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
    shaderStages[0] = loadShader(FS::getAssetPath("shaders/09_VolumeFog/volumefog.so.vert"), VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] = loadShader(FS::getAssetPath("shaders/09_VolumeFog/volumefog.so.frag"), VK_SHADER_STAGE_FRAGMENT_BIT);
    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_volumeSmoke->m_pipeline));
}

void VolumeSmoke::buildCommandBuffers(){
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

        VkViewport viewports = { 0, 0, 1.f*width, 1.f*height, 0.0, 1.0f };
        VkRect2D scissorRects=vks::initializers::rect2D(width, height, 0, 0);

        vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewports);
        vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissorRects);

        m_volumeSmoke->build(drawCmdBuffers[i]);

        drawUI(drawCmdBuffers[i]);

        vkCmdEndRenderPass(drawCmdBuffers[i]);
        VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
    vkQueueWaitIdle(queue);
}
