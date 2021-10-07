//
// Created by wjl on 21-5-23.
//

#include "VulkanFrameBuffer.h"
#include "VulkanInitializers.hpp"
#include "VulkanTools.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanFrameBuffer::~VulkanFrameBuffer() {
    delete_ptr(m_renderPass);

    // Frame buffer
    VK_SAFE_DELETE(m_depthSampler, vkDestroySampler(m_device, m_depthSampler, nullptr));

    VK_SAFE_DELETE(m_color.view, vkDestroyImageView(m_device, m_color.view, nullptr));
    VK_SAFE_DELETE(m_color.image, vkDestroyImage(m_device, m_color.image, nullptr));
    VK_SAFE_DELETE(m_color.memory, vkFreeMemory(m_device, m_color.memory, nullptr));

    VK_SAFE_DELETE(m_depth.view, vkDestroyImageView(m_device, m_depth.view, nullptr));
    VK_SAFE_DELETE(m_depth.image, vkDestroyImage(m_device, m_depth.image, nullptr));
    VK_SAFE_DELETE(m_depth.memory, vkFreeMemory(m_device, m_depth.memory, nullptr));

    vkDestroyFramebuffer(m_device, m_frameBuffer, nullptr);
}

void VulkanFrameBuffer::createWithDepth() {
    // For shadow mapping we only need a depth attachment
    VkImageCreateInfo image = vks::initializers::imageCreateInfo();
    image.imageType = VK_IMAGE_TYPE_2D;
    image.extent.width = m_width;
    image.extent.height = m_height;
    image.extent.depth = 1;
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    image.format = m_format;																// Depth stencil attachment
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;		// We will sample directly from the depth attachment for the shadow mapping
    VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_depth.image));

    VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(m_device, m_depth.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_depth.memory));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depth.image, m_depth.memory, 0));

    VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = m_format;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = m_depth.image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depth.view));

    // Create sampler to sample from to depth attachment
    // Used to sample in the fragment shader for shadowed rendering
    VkFilter shadowmap_filter = VK_FILTER_LINEAR;
    VkSamplerCreateInfo sampler = vks::initializers::samplerCreateInfo();
    sampler.magFilter = shadowmap_filter;
    sampler.minFilter = shadowmap_filter;
    sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    sampler.addressModeV = sampler.addressModeU;
    sampler.addressModeW = sampler.addressModeU;
    sampler.mipLodBias = 0.0f;
    sampler.maxAnisotropy = 1.0f;
    sampler.minLod = 0.0f;
    sampler.maxLod = 1.0f;
    sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VK_CHECK_RESULT(vkCreateSampler(m_device, &sampler, nullptr, &m_depthSampler));

    //prepareOffscreenRenderpass();
    m_renderPass = new VulkanRenderPass();
    m_renderPass->setDevice(m_device);
    m_renderPass->setFormat(m_format);
    m_renderPass->createDepthPass();

    // Create frame buffer
    VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
    fbufCreateInfo.renderPass = m_renderPass->get();
    fbufCreateInfo.attachmentCount = 1;
    fbufCreateInfo.pAttachments = &m_depth.view;
    fbufCreateInfo.width = m_width;
    fbufCreateInfo.height = m_height;
    fbufCreateInfo.layers = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &fbufCreateInfo, nullptr, &m_frameBuffer));

    m_descriptor = vks::initializers::descriptorImageInfo(
            m_depthSampler,
            m_depth.view,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}

void VulkanFrameBuffer::createWithColorDepth() {
    // Find a suitable depth format
    VkFormat fbDepthFormat;
    VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(m_context->getPhysicalDevice(), &fbDepthFormat);
    assert(validDepthFormat);

    // Color attachment
    VkImageCreateInfo image = vks::initializers::imageCreateInfo();
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = m_format;
    image.extent.width = m_width;
    image.extent.height = m_height;
    image.extent.depth = 1;
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    // We will sample directly from the color attachment
    image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
    VkMemoryRequirements memReqs;

    VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_color.image));
    vkGetImageMemoryRequirements(m_device, m_color.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex =
            m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_color.memory));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_color.image, m_color.memory, 0));

    VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
    colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    colorImageView.format = m_format;
    colorImageView.subresourceRange = {};
    colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorImageView.subresourceRange.baseMipLevel = 0;
    colorImageView.subresourceRange.levelCount = 1;
    colorImageView.subresourceRange.baseArrayLayer = 0;
    colorImageView.subresourceRange.layerCount = 1;
    colorImageView.image = m_color.image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &colorImageView, nullptr, &m_color.view));

    // Create sampler to sample from the attachment in the fragment shader
    VkSamplerCreateInfo samplerInfo = vks::initializers::samplerCreateInfo();
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = samplerInfo.addressModeU;
    samplerInfo.addressModeW = samplerInfo.addressModeU;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VK_CHECK_RESULT(vkCreateSampler(m_device, &samplerInfo, nullptr, &m_colorSampler));

    // Depth stencil attachment
    image.format = fbDepthFormat;
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_depth.image));
    vkGetImageMemoryRequirements(m_device, m_depth.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex =
            m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_depth.memory));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depth.image, m_depth.memory, 0));

    VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = fbDepthFormat;
    // depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = m_depth.image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depth.view));

    // Create a separate render pass for the offscreen rendering as it may differ from the one used for scene rendering

    std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};
    // Color attachment
    attchmentDescriptions[0].format = m_format;
    attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    // Depth attachment
    attchmentDescriptions[1].format = fbDepthFormat;
    attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkAttachmentReference depthReference = {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;

    // Use subpass dependencies for layout transitions
    std::array<VkSubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    // Create the actual renderpass
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
    renderPassInfo.pAttachments = attchmentDescriptions.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    VK_CHECK_RESULT(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_pRenderPass));

    VkImageView attachments[2];
    attachments[0] = m_color.view;
    attachments[1] = m_depth.view;

    VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
    fbufCreateInfo.renderPass = m_pRenderPass;
    fbufCreateInfo.attachmentCount = 2;
    fbufCreateInfo.pAttachments = attachments;
    fbufCreateInfo.width = m_width;
    fbufCreateInfo.height = m_height;
    fbufCreateInfo.layers = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &fbufCreateInfo, nullptr, &m_frameBuffer));

    // Fill a descriptor for later use in a descriptor set
    m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    m_descriptor.imageView = m_color.view;
    m_descriptor.sampler = m_colorSampler;

    VkSamplerCreateInfo samplerInfoDepth = vks::initializers::samplerCreateInfo();
    samplerInfoDepth.magFilter = VK_FILTER_LINEAR;
    samplerInfoDepth.minFilter = VK_FILTER_LINEAR;
    samplerInfoDepth.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfoDepth.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfoDepth.addressModeV = samplerInfoDepth.addressModeU;
    samplerInfoDepth.addressModeW = samplerInfoDepth.addressModeU;
    samplerInfoDepth.mipLodBias = 0.0f;
    samplerInfoDepth.maxAnisotropy = 1.0f;
    samplerInfoDepth.minLod = 0.0f;
    samplerInfoDepth.maxLod = 1.0f;
    samplerInfoDepth.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VK_CHECK_RESULT(vkCreateSampler(m_device, &samplerInfoDepth, nullptr, &m_depthSampler));

    m_descriptor = vks::initializers::descriptorImageInfo(m_colorSampler, m_color.view,
                                                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

//    m_depthDescriptor = vks::initializers::descriptorImageInfo(m_samplerDepth, m_depth.view,
//                                                               VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}

END_NAMESPACE(VulkanEngine)