//
// Created by wjl on 21-2-17.
//

#include "VulkanBase.h"

BEGIN_NAMESPACE(VulkanEngine)
VulkanBase::~VulkanBase() {
    VK_CHECK_RESULT(vkQueueWaitIdle(m_queue));
    vkDeviceWaitIdle(m_device);
    VK_SAFE_DELETE(m_descriptorPool, vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr));
    VK_SAFE_DELETE(m_renderPass, vkDestroyRenderPass(m_device, m_renderPass, nullptr));
    for (auto &shaderModule : m_shaderModules) {
        VK_SAFE_DELETE(shaderModule, vkDestroyShaderModule(m_device, shaderModule, nullptr));
    }
    VK_SAFE_DELETE(m_pipelineCache, vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr));

    VK_SAFE_DELETE(m_cmdPool, vkDestroyCommandPool(m_device, m_cmdPool, nullptr));

    VK_SAFE_DELETE(m_semaphores.presentComplete, vkDestroySemaphore(m_device, m_semaphores.presentComplete, nullptr));
    VK_SAFE_DELETE(m_semaphores.renderComplete, vkDestroySemaphore(m_device, m_semaphores.renderComplete, nullptr));
    for (auto &fence : m_waitFences) {
        VK_SAFE_DELETE(fence, vkDestroyFence(m_device, fence, nullptr));
    }
    destroySurface();
    delete_ptr(m_vulkanDevice);
    VK_SAFE_DELETE(m_instance, vkDestroyInstance(m_instance, nullptr));
    destroyANativeWindow();
}

void VulkanBase::initVulkan() {
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
}

void VulkanBase::prepare() {
    prepareBase();
}

void VulkanBase::prepareBase() {
    initSwapchain();
    createCommandPool();
    setupSwapChain();
    createCommandBuffers();
    createSynchronizationPrimitives();
    setupDepthStencil();
    setupRenderPass();
    createPipelineCache();
    setupFrameBuffer();
}

void VulkanBase::renderLoop() {}

void VulkanBase::renderFrame() {
    if (m_prepared and !m_pause) {
        render();
        draw();
        vkDeviceWaitIdle(m_device);
    }
}

void VulkanBase::render() {}

void VulkanBase::draw() {
    if (m_stop or m_pause) return;
    m_signalFrame = false;
    prepareFrame();

    // Command buffer to be sumitted to the queue
    m_submitInfo.commandBufferCount = 1;
    m_submitInfo.pCommandBuffers = &m_drawCmdBuffers[m_currentBuffer];

    // Submit to queue
    if(m_prepared){
        VK_CHECK_RESULT(vkQueueSubmit(m_queue, 1, &m_submitInfo, VK_NULL_HANDLE));
    }

    submitFrame();
    m_signalFrame = true;
}

void VulkanBase::defaultTouchOperation() {
    if (m_touchMode == TouchMode::SINGLE) {
        if (m_mousePosOld[0].x == 0 and m_mousePosOld[0].y == 0) {
            m_mousePosOld[0].x = m_mousePos[0].x;
            m_mousePosOld[0].y = m_mousePos[0].y;
        }
    } else if (m_touchMode == TouchMode::DOUBLE) {
        float distance =
                (m_mousePos[1].x - m_mousePos[0].x) * (m_mousePos[1].x - m_mousePos[0].x) +
                (m_mousePos[1].y - m_mousePos[0].y) * (m_mousePos[1].y - m_mousePos[0].y);
        if (m_oldDistance == 0.f) { m_oldDistance = distance; }
        else {
            if (distance > m_oldDistance) { m_distance += 0.1f; }
            else if (distance < m_oldDistance) { m_distance -= 0.1f; }
        }
        m_oldDistance = distance;
    }
}

void VulkanBase::waitForCurrentFrameComplete() {
    m_pause = true;
    while(m_signalFrame == false){
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void VulkanBase::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "LearnVulkan";
    appInfo.pEngineName = "LearnVulkan";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};

    instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

    if (m_enabledInstanceExtensions.size() > 0) {
        for (auto enabledExtension : m_enabledInstanceExtensions) {
            instanceExtensions.push_back(enabledExtension);
        }
    }

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if (instanceExtensions.size() > 0) {
        if (m_debug) {
            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t) instanceExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    }
    if (m_debug) {
        // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
        // Note that on Android this layer requires at least NDK r20
        const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
        // Check if this layer is available at instance level
        uint32_t instanceLayerCount;
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
        bool validationLayerPresent = false;
        for (VkLayerProperties layer : instanceLayerProperties) {
            if (strcmp(layer.layerName, validationLayerName) == 0) {
                validationLayerPresent = true;
                break;
            }
        }
        if (validationLayerPresent) {
            instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
            instanceCreateInfo.enabledLayerCount = 1;
        } else {
            LOGI("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
        }
    }
    m_result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
}

void VulkanBase::pickPhysicalDevice() {
    uint32_t gpuCount = 0;
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr));
    assert(gpuCount > 0);
    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    m_result = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());

    // GPU selection

    // Select physical device to be used for the Vulkan example
    // Defaults to the first device unless specified by command line
    uint32_t selectedDevice = 0;

    m_physicalDevice = physicalDevices[selectedDevice];

    // Store properties (including limits), features and memory properties of the phyiscal device (so that examples can check against them)
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);

    // Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
    getDeviceFeatures();
}

void VulkanBase::createLogicalDevice(){
    m_enabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    m_vulkanDevice = new vks::VulkanDevice(m_physicalDevice);
    m_result = m_vulkanDevice->createLogicalDevice(m_enabledFeatures, m_enabledDeviceExtensions,
                                                   m_deviceCreatepNextChain);
    m_device = m_vulkanDevice->logicalDevice;

    vkGetDeviceQueue(m_device, m_vulkanDevice->queueFamilyIndices.graphics, 0, &m_queue);

    // Find a suitable depth format
    VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(m_physicalDevice, &m_depthFormat);
    assert(validDepthFormat);

    m_swapChain.connect(m_instance, m_physicalDevice, m_device);

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queu
    VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.presentComplete));
    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been sumbitted and executed
    VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.renderComplete));

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    m_submitInfo = vks::initializers::submitInfo();
    m_submitInfo.pWaitDstStageMask = &m_submitPipelineStages;
    m_submitInfo.waitSemaphoreCount = 1;
    m_submitInfo.pWaitSemaphores = &m_semaphores.presentComplete;
    m_submitInfo.signalSemaphoreCount = 1;
    m_submitInfo.pSignalSemaphores = &m_semaphores.renderComplete;
}

void VulkanBase::initSwapchain(){
    m_swapChain.initSurface(m_window);
}

void VulkanBase::createCommandPool(){
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = m_swapChain.queueNodeIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(vkCreateCommandPool(m_device, &cmdPoolInfo, nullptr, &m_cmdPool));
}

void VulkanBase::setupSwapChain(){
    m_swapChain.create(&m_width, &m_height, false);
};

void VulkanBase::createCommandBuffers(){
    m_drawCmdBuffers.resize(m_swapChain.imageCount);

    VkCommandBufferAllocateInfo cmdBufAllocateInfo =
            vks::initializers::commandBufferAllocateInfo(
                    m_cmdPool,
                    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                    static_cast<uint32_t>(m_drawCmdBuffers.size()));

    VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, m_drawCmdBuffers.data()));
}

void VulkanBase::createSynchronizationPrimitives(){
    VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
    m_waitFences.resize(m_drawCmdBuffers.size());
    for (auto &fence : m_waitFences) {
        VK_CHECK_RESULT(vkCreateFence(m_device, &fenceCreateInfo, nullptr, &fence));
    }
}

void VulkanBase::setupDepthStencil(){
    VkImageCreateInfo imageCI{};
    imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCI.imageType = VK_IMAGE_TYPE_2D;
    imageCI.format = m_depthFormat;
    imageCI.extent = {m_width, m_height, 1};
    imageCI.mipLevels = 1;
    imageCI.arrayLayers = 1;
    imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VK_CHECK_RESULT(vkCreateImage(m_device, &imageCI, nullptr, &m_depthStencil.image));
    VkMemoryRequirements memReqs{};
    vkGetImageMemoryRequirements(m_device, m_depthStencil.image, &memReqs);

    VkMemoryAllocateInfo memAllloc{};
    memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllloc.allocationSize = memReqs.size;
    memAllloc.memoryTypeIndex = m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits,
                                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAllloc, nullptr, &m_depthStencil.mem));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depthStencil.image, m_depthStencil.mem, 0));

    VkImageViewCreateInfo imageViewCI{};
    imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCI.image = m_depthStencil.image;
    imageViewCI.format = m_depthFormat;
    imageViewCI.subresourceRange.baseMipLevel = 0;
    imageViewCI.subresourceRange.levelCount = 1;
    imageViewCI.subresourceRange.baseArrayLayer = 0;
    imageViewCI.subresourceRange.layerCount = 1;
    imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    // Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
    if (m_depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
        imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    VK_CHECK_RESULT(vkCreateImageView(m_device, &imageViewCI, nullptr, &m_depthStencil.view));
}

void VulkanBase::setupRenderPass(){
    std::array<VkAttachmentDescription, 2> attachments = {};
    // Color attachment
    attachments[0].format = m_swapChain.colorFormat;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    // Depth attachment
    attachments[1].format = m_depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;
    subpassDescription.pResolveAttachments = nullptr;

    // Subpass dependencies for layout transitions
    std::array<VkSubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass));
}

void VulkanBase::createPipelineCache() {
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VK_CHECK_RESULT(vkCreatePipelineCache(m_device, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
}

void VulkanBase::setupFrameBuffer(){
    VkImageView attachments[2];

    // Depth/Stencil attachment is the same for all frame buffers
    attachments[1] = m_depthStencil.view;

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = NULL;
    frameBufferCreateInfo.renderPass = m_renderPass;
    frameBufferCreateInfo.attachmentCount = 2;
    frameBufferCreateInfo.pAttachments = attachments;
    frameBufferCreateInfo.width = m_width;
    frameBufferCreateInfo.height = m_height;
    frameBufferCreateInfo.layers = 1;

    // Create frame buffers for every swap chain image
    m_frameBuffers.resize(m_swapChain.imageCount);
    for (uint32_t i = 0; i < m_frameBuffers.size(); i++) {
        attachments[0] = m_swapChain.buffers[i].view;
        VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, nullptr, &m_frameBuffers[i]));
    }
}

void VulkanBase::prepareFrame(){
    if (m_pause or !m_prepared) {
        return;
    }
    // Acquire the next image from the swap chain
    VkResult err = m_swapChain.acquireNextImage(m_semaphores.presentComplete, &m_currentBuffer);
    // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
    if ((err == VK_ERROR_OUT_OF_DATE_KHR) || (err == VK_SUBOPTIMAL_KHR)) {
        destroySurface();
        rebuildSurface();
        LOGI("VulkanEngine VK_ERROR_OUT_OF_DATE_KHR");
    } else {
        VK_CHECK_RESULT(err);
    }
    VK_CHECK_RESULT(vkQueueWaitIdle(m_queue));
}

void VulkanBase::submitFrame() {
    if (m_pause) {
        return;
    }
    VkResult res = m_swapChain.queuePresent(m_queue, m_currentBuffer, m_semaphores.renderComplete);
    if (!((res == VK_SUCCESS) || (res == VK_SUBOPTIMAL_KHR))) {
        if (res == VK_ERROR_OUT_OF_DATE_KHR) {
            // Swap chain is no longer compatible with the surface and needs to be recreated
            destroySurface();
            rebuildSurface();
            LOGI("VulkanEngine VK_ERROR_OUT_OF_DATE_KHR");
            return;
        } else {
            VK_CHECK_RESULT(res);
        }
    }
    VK_CHECK_RESULT(vkQueueWaitIdle(m_queue));
}

void VulkanBase::destroySurface() {
    if (!m_prepared) {
        return;
    }
    LOGI("destroySurface");
    m_swapChain.cleanup();
    destroyCommandBuffers();
//    if(m_window){
//        ANativeWindow_release(m_window);
//        m_window = nullptr;
//    }
    VK_SAFE_DELETE(m_depthStencil.view, vkDestroyImageView(m_device, m_depthStencil.view, nullptr));
    VK_SAFE_DELETE(m_depthStencil.image, vkDestroyImage(m_device, m_depthStencil.image, nullptr));
    VK_SAFE_DELETE(m_depthStencil.mem, vkFreeMemory(m_device, m_depthStencil.mem, nullptr));
    for (uint32_t i = 0; i < m_frameBuffers.size(); i++) {
        VK_SAFE_DELETE(m_frameBuffers[i], vkDestroyFramebuffer(m_device, m_frameBuffers[i], nullptr));
    }
    m_prepared = false;
}

void VulkanBase::destroyCommandBuffers(){
    if(m_drawCmdBuffers.size()>0){
        vkFreeCommandBuffers(m_device, m_cmdPool, static_cast<uint32_t>(m_drawCmdBuffers.size()), m_drawCmdBuffers.data());
    }
    m_drawCmdBuffers.resize(0);
}

void VulkanBase::rebuildSurface() {
    if(m_window == nullptr) return;
    m_swapChain.connect(m_instance, m_physicalDevice, m_device);
    initSwapchain();
    setupSwapChain();
    setupDepthStencil();
    setupFrameBuffer();
    createCommandBuffers();
    buildCommandBuffers();

    vkDeviceWaitIdle(m_device);

    m_prepared = true;
}

void VulkanBase::destroyANativeWindow() {
    if(m_window){
        ANativeWindow_release(m_window);
        m_window = nullptr;
    }
}

END_NAMESPACE(VulkanEngine)