//
// Created by wjl on 21-2-23.
//

#include "VulkanBaseEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanBaseEngine::~VulkanBaseEngine() {
    delete_ptr(m_vulkanDescriptorSet);
    delete_ptr(m_vulkanVertexDescriptions);
    delete_ptr(m_pipelines);
    delete_ptr(m_context);
    VK_SAFE_DELETE(m_pipelineLayout, vkDestroyPipelineLayout(m_device,m_pipelineLayout, nullptr));
}

void VulkanBaseEngine::prepare() {
    prepareBase();
    prepareDescriptorSets();
    prepareVertexDescriptions();
    prepareBasePipelines();
    prepareContext();
    prepareMyObjects();
    buildCommandBuffers();
    m_prepared = true;
}

void VulkanBaseEngine::render() {}

void VulkanBaseEngine::prepareDescriptorSets() {
    m_vulkanDescriptorSet = new VulkanDescriptorSet(m_device,m_maxSets);
}

void VulkanBaseEngine::prepareVertexDescriptions(){
    m_vulkanVertexDescriptions = new VulkanVertexDescriptions();
    m_vulkanVertexDescriptions->GenerateTexVec4Descriptions();
}

void VulkanBaseEngine::prepareBasePipelines() {
    m_pipelines = new VulkanPipelines(m_device);
    m_pipelines->m_vertexInputState = m_vulkanVertexDescriptions->m_inputState;
    m_pipelines->m_pipelineCache = m_pipelineCache;
}

void VulkanBaseEngine::prepareContext() {
    m_context = new VulkanContext();
    m_context->vulkanDevice = m_vulkanDevice;
    m_context->cmdPool = m_cmdPool;
    m_context->pPipelineLayout = &m_pipelineLayout;
    m_context->pipelineCache = m_pipelineCache;
    m_context->renderPass = m_renderPass;
    m_context->queue = m_queue;
    m_context->pScreenWidth = &m_width;
    m_context->pScreenHeight = &m_height;
    m_context->m_asset = m_asset;
}

void VulkanBaseEngine::buildCommandBuffers() {
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    for (size_t i = 0; i < m_drawCmdBuffers.size(); ++i) {

        LOGI("m_drawCmdBuffers %ld",m_drawCmdBuffers[i]);
        VK_CHECK_RESULT(vkBeginCommandBuffer(m_drawCmdBuffers[i], &cmdBufInfo));

        buildCommandBuffersBeforeMainRenderPass(m_drawCmdBuffers[i]);

        {
            VkClearValue clearValues[2];
            clearValues[0].color = {{0.1f, 0.2f, 0.3f, 1.0f}};
            clearValues[1].depthStencil = {1.0f, 0};
            // Set target frame buffer
            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = m_renderPass;
            renderPassBeginInfo.renderArea.offset.x = 0;
            renderPassBeginInfo.renderArea.offset.y = 0;
            renderPassBeginInfo.renderArea.extent.width = m_width;
            renderPassBeginInfo.renderArea.extent.height = m_height;
            renderPassBeginInfo.clearValueCount = 2;
            renderPassBeginInfo.pClearValues = clearValues;
            renderPassBeginInfo.framebuffer = m_frameBuffers[i];

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkDeviceSize offsets[1] = {0};
            vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
                                    &(m_vulkanDescriptorSet->get(0)), 0, NULL);

            setViewPorts(m_drawCmdBuffers[i]);
            buildMyObjects(m_drawCmdBuffers[i]);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
        }

        buildCommandBuffersAfterMainRenderPass(m_drawCmdBuffers[i]);

        VK_CHECK_RESULT(vkEndCommandBuffer(m_drawCmdBuffers[i]));
    }
    vkQueueWaitIdle(m_queue);
}

void VulkanBaseEngine::setViewPorts(VkCommandBuffer &cmd) {
    VkViewport viewports[1];
    VkRect2D scissorRects[1];

    viewports[0] = {0, 0, float(m_width), float(m_height), 0.0, 1.0};
    scissorRects[0] = vks::initializers::rect2D(m_width, m_height, 0, 0);

    vkCmdSetViewport(cmd, 0, 1, viewports);
    vkCmdSetScissor(cmd, 0, 1, scissorRects);
}

END_NAMESPACE(VulkanEngine)