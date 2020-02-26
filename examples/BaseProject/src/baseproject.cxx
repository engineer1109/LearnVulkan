#include "baseproject.h"
BaseProject::BaseProject(bool debug)
    : VulkanBasicEngine(debug) {
    // define your title
    title = "Base Project";
    // open or close overlay IMGUI
    settings.overlay = true;
    // zoom is the base para of camera
    zoom = 4.f;
}

BaseProject::~BaseProject() {
    // delete object class here.
}

void BaseProject::prepare() {
    // parent base prepare for renderpass
    VulkanBasicEngine::prepare();
    // your prepare
    createObjects();
    setupDescriptorPool();
    setupDescriptorSetLayout();
    setupDescriptorSet();
    preparePipelines();
    buildCommandBuffers();
    prepared = true;
}

void BaseProject::render() {
    if (!prepared) return;
    draw();

    if (frameCounter == 0) {
        vkDeviceWaitIdle(device);
    }
}

void BaseProject::draw() {
    // Parent class function for swapChain.
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void BaseProject::OnUpdateUIOverlay(vks::UIOverlay *overlay) {
    // Design your IMGUI here.
    if (overlay->header("Example Project")) {
        overlay->text("Hello Vulkan.");
    }
}

void BaseProject::getEnabledFeatures() {
    // Other features enabled
    // Enable anisotropic filtering if supported
    if (deviceFeatures.samplerAnisotropy) {
        enabledFeatures.samplerAnisotropy = VK_TRUE;
    }
}

void BaseProject::createObjects() {
    // Prepare your objects
}

void BaseProject::setupDescriptorPool() {
    // prepare DescriptorPool
}

void BaseProject::setupDescriptorSetLayout() {
    // Binding
}

void BaseProject::setupDescriptorSet() {
    // setup descriptor
}

void BaseProject::preparePipelines() {
    // main pipeline
}

void BaseProject::buildCommandBuffers() {
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    // BackGround Color
    VkClearValue clearValues[2];
    clearValues[0].color = {{0.1f, 0.2f, 0.3f, 0.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    // RenderPassBegin
    VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = width;
    renderPassBeginInfo.renderArea.extent.height = height;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    for (int32_t i = 0; i < drawCmdBuffers.size(); ++i) {
        // Set target frame buffer
        renderPassBeginInfo.framebuffer = frameBuffers[i];

        VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo));

        vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkDeviceSize offsets[1] = {0};

        VkViewport viewports = {0, 0, 1.f * width, 1.f * height, 0.0, 1.0f};
        VkRect2D scissorRects = vks::initializers::rect2D(width, height, 0, 0);

        vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewports);
        vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissorRects);

        // Add your objects here.

        // Draw IMGUI
        drawUI(drawCmdBuffers[i]);

        vkCmdEndRenderPass(drawCmdBuffers[i]);
        VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
    vkQueueWaitIdle(queue);
}
