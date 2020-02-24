/*
 * LearnVulkan Examples
 *
 * Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "instancedraw.h"
#include "instancecube.h"
#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1
InstanceDraw::InstanceDraw(bool debug)
    : VulkanBasicEngine(debug) {
    title = "InstanceDraw";
    settings.overlay = true;
}

InstanceDraw::~InstanceDraw() {
    if (m_cube != nullptr) {
        delete m_cube;
        m_cube = nullptr;
    }
    vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
}

void InstanceDraw::prepare() {
    VulkanBasicEngine::prepare();
    createObjects();
    setupDescriptorPool();
    setupDescriptorSetLayout();
    setupDescriptorSet();
    preparePipelines();
    buildCommandBuffers();
    prepared = true;
}

void InstanceDraw::render() {
    if (!prepared) return;
    draw();
    m_cube->update();
    // std::cout << zoom << std::endl;

    if (frameCounter == 0) {
        vkDeviceWaitIdle(device);
    }
}

void InstanceDraw::draw() {
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void InstanceDraw::OnUpdateUIOverlay(vks::UIOverlay *overlay) {
    if (overlay->header("Statistics")) {
        overlay->text("Instances Draw : 100 cubes");
    }
}

void InstanceDraw::getEnabledFeatures() {
    // Enable anisotropic filtering if supported
    if (deviceFeatures.samplerAnisotropy) {
        enabledFeatures.samplerAnisotropy = VK_TRUE;
    }
}

void InstanceDraw::createObjects() {
    VulkanTemplate::VulkanBaseObject::ObjectInfo objectinfo;
    objectinfo.vulkanDevice = vulkanDevice;
    objectinfo.instance = instance;
    objectinfo.cmdPool = cmdPool;
    objectinfo.pPipelineLayout = &m_pipelineLayout;
    objectinfo.pPipelineCache = &pipelineCache;
    objectinfo.pRenderPass = &renderPass;
    objectinfo.queue = queue;
    objectinfo.screenWitdh = &width;
    objectinfo.screenHeight = &height;

    InstanceCube::ObjectCamera camera;
    camera.zoom = &zoom;
    camera.rotation = &rotation;
    camera.cameraPos = &cameraPos;

    m_cube = new InstanceCube();
    m_cube->setObjectInfo(objectinfo);
    m_cube->setCamera(camera);
    m_cube->create();
}

void InstanceDraw::setupDescriptorPool() {
    // Example uses one ubo
    std::vector<VkDescriptorPoolSize> poolSizes = {
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
        vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1),
    };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
        vks::initializers::descriptorPoolCreateInfo(poolSizes.size(), poolSizes.data(), 1);

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
}

void InstanceDraw::setupDescriptorSetLayout() {
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
        // Binding 0 : Vertex shader uniform buffer
        vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
        // Binding 1 : Fragment shader combined sampler
        vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                      VK_SHADER_STAGE_FRAGMENT_BIT, 1),
    };

    VkDescriptorSetLayoutCreateInfo descriptorLayout =
        vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings.data(), setLayoutBindings.size());

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &m_descriptorSetLayout));

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo =
        vks::initializers::pipelineLayoutCreateInfo(&m_descriptorSetLayout, 1);

    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
}

void InstanceDraw::setupDescriptorSet() {
    VkDescriptorSetAllocateInfo allocInfo =
        vks::initializers::descriptorSetAllocateInfo(descriptorPool, &m_descriptorSetLayout, 1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &m_cube->m_descriptorSet));

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;

    // Binding 0: Vertex shader uniform buffer
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
        m_cube->m_descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &(m_cube->m_uniformBufferVS).descriptor));

    // Binding 1: Sampled image
    VkDescriptorImageInfo textureDescriptor =
        vks::initializers::descriptorImageInfo(VK_NULL_HANDLE, m_cube->m_texture.view, m_cube->m_texture.imageLayout);
    writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(
        m_cube->m_descriptorSet, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, &textureDescriptor));

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0,
                           NULL);
}

void InstanceDraw::preparePipelines() {
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
        vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);

    VkPipelineRasterizationStateCreateInfo rasterizationState = vks::initializers::pipelineRasterizationStateCreateInfo(
        VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, 0);

    VkPipelineColorBlendAttachmentState blendAttachmentState =
        vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);

    VkPipelineColorBlendStateCreateInfo colorBlendState =
        vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);

    VkPipelineDepthStencilStateCreateInfo depthStencilState =
        vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);

    VkPipelineViewportStateCreateInfo viewportState = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);

    VkPipelineMultisampleStateCreateInfo multisampleState =
        vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, 0);

    std::vector<VkDynamicState> dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState =
        vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables.data(), dynamicStateEnables.size(), 0);

    // Load shaders
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo =
        vks::initializers::pipelineCreateInfo(m_pipelineLayout, renderPass, 0);

    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = shaderStages.size();
    pipelineCreateInfo.pStages = shaderStages.data();

    pipelineCreateInfo.pVertexInputState = &m_cube->m_vertices.inputState;

    shaderStages[0] =
        loadShader(FS::getAssetPath("shaders/08_InstanceDraw/instance.so.vert"), VK_SHADER_STAGE_VERTEX_BIT);
    shaderStages[1] =
        loadShader(FS::getAssetPath("shaders/08_InstanceDraw/instance.so.frag"), VK_SHADER_STAGE_FRAGMENT_BIT);
    VK_CHECK_RESULT(
        vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &(m_cube->m_pipeline)));
}

void InstanceDraw::buildCommandBuffers() {
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    VkClearValue clearValues[2];
    clearValues[0].color = {{0.0f, 0.0f, 0.2f, 0.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.renderArea.extent.width = width;
    renderPassBeginInfo.renderArea.extent.height = height;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    for (int32_t i = 0; i < drawCmdBuffers.size(); ++i) {
        // Set target frame buffer
        renderPassBeginInfo.framebuffer = frameBuffers[i];

        VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo));

        vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
        vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);

        VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);
        vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissor);

        VkDeviceSize offsets[1] = {0};

        m_cube->build(drawCmdBuffers[i], m_pipelineLayout);

        drawUI(drawCmdBuffers[i]);

        vkCmdEndRenderPass(drawCmdBuffers[i]);

        VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
}
