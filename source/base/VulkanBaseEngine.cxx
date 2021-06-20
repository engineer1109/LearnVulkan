//
// Created by wjl on 21-2-23.
//

#include "VulkanBaseEngine.h"

#include "filesystem_utils.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanBaseEngine::~VulkanBaseEngine() {
#ifndef __ANDROID__
    m_UIOverlay.freeResources();
#endif
    delete_ptr(m_vulkanDescriptorSet);
    delete_ptr(m_vulkanVertexDescriptions);
    delete_ptr(m_pipelines);
    delete_ptr(m_context);
    VK_SAFE_DELETE(m_pipelineLayout, vkDestroyPipelineLayout(m_device,m_pipelineLayout, nullptr));
}

void VulkanBaseEngine::initWindow() {
#if defined(VK_USE_PLATFORM_XCB_KHR)
    initxcbConnection();
    setupWindow();
#elif defined(_WIN32)
    setupWindow();
#endif
}

void VulkanBaseEngine::prepare() {
    prepareBase();
    prepareDescriptorSets();
    prepareVertexDescriptions();
    prepareBasePipelines();
    prepareContext();
    prepareIMGUI();
    prepareMyObjects();
    buildCommandBuffers();
    m_prepared = true;
}

void VulkanBaseEngine::render() {}

void VulkanBaseEngine::updateOverlay() {
#ifndef __ANDROID__
    if (!m_settings.overlay)
        return;

    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)m_width, (float)m_height);
    io.DeltaTime = m_frameTimer;

    io.MousePos = ImVec2(m_mousePos.x, m_mousePos.y);
    io.MouseDown[0] = m_mouseButtons.left;
    io.MouseDown[1] = m_mouseButtons.right;

    ImGui::NewFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::Begin("Vulkan Example", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::TextUnformatted(m_title.c_str());
    ImGui::TextUnformatted(m_deviceProperties.deviceName);
    ImGui::Text("%.2f ms/frame (%.1d fps)", m_frameTimer * 1000, int(1.f / m_frameTimer));

    ImGui::PushItemWidth(110.0f * m_UIOverlay.scale);
    OnUpdateUIOverlay(&m_UIOverlay);
    ImGui::PopItemWidth();

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::Render();

    if (m_UIOverlay.update() || m_UIOverlay.updated) {
        buildCommandBuffers();
        m_UIOverlay.updated = false;
    }

#endif
}

void VulkanBaseEngine::drawUI(const VkCommandBuffer commandBuffer) {
#ifndef __ANDROID__
    if (m_settings.overlay) {
        const VkViewport viewport = vks::initializers::viewport((float)m_width, (float)m_height, 0.0f, 1.0f);
        const VkRect2D scissor = vks::initializers::rect2D(m_width, m_height, 0, 0);
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        m_UIOverlay.draw(commandBuffer);
    }
#endif
}

void VulkanBaseEngine::updateCommand() {
    if(m_rebuild) {
        buildCommandBuffers();
        m_rebuild = false;
    }
}

void VulkanBaseEngine::prepareIMGUI() {
#ifndef __ANDROID__
    if(m_settings.overlay){
        m_UIOverlay.device = m_vulkanDevice;
        m_UIOverlay.queue = m_queue;
        m_UIOverlay.shaders = {
                loadShader(FS::getPath("shaders/base/uioverlay.vert.spv"), VK_SHADER_STAGE_VERTEX_BIT),
                loadShader(FS::getPath("shaders/base/uioverlay.frag.spv"), VK_SHADER_STAGE_FRAGMENT_BIT),
        };
        m_UIOverlay.prepareResources();
        m_UIOverlay.preparePipeline(m_pipelineCache, m_renderPass);
    }
#endif
}

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
}

void VulkanBaseEngine::buildCommandBuffers() {
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    for (size_t i = 0; i < m_drawCmdBuffers.size(); ++i) {

        //LOGI("m_drawCmdBuffers %ld\n",m_drawCmdBuffers[i]);
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

            drawUI(m_drawCmdBuffers[i]);

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