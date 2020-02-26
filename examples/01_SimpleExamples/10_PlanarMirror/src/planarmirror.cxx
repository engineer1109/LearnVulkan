#include "planarmirror.h"
PlanarMirror::PlanarMirror(bool debug)
    : VulkanBasicEngine(debug) {
    zoom = -6.0f;
    title = "offscreen render of mirror";
    settings.overlay = true;
    enabledFeatures.shaderClipDistance = VK_TRUE;
}

PlanarMirror::~PlanarMirror() {}

void PlanarMirror::prepare() {
    VulkanBasicEngine::prepare();
    buildCommandBuffers();
    prepared = true;
}

void PlanarMirror::render() {
    if (!prepared) return;
    draw();

    if (frameCounter == 0) {
        vkDeviceWaitIdle(device);
    }
}

void PlanarMirror::draw() {
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void PlanarMirror::OnUpdateUIOverlay(vks::UIOverlay *overlay) {
    if (overlay->header("Test Mirror")) {
        overlay->text("test");
    }
}

void PlanarMirror::getEnabledFeatures() {}

void PlanarMirror::buildCommandBuffers() {
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

        drawUI(drawCmdBuffers[i]);

        vkCmdEndRenderPass(drawCmdBuffers[i]);

        VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
}
