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
    // Color attachment
    VkImageCreateInfo imageCreateInfo = vks::initializers::imageCreateInfo();
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = m_format;
    imageCreateInfo.extent.width = m_width;
    imageCreateInfo.extent.height = m_height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    // Image of the framebuffer is blit source
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();

    VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
    colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    colorImageView.format = m_format;
    colorImageView.flags = 0;
    colorImageView.subresourceRange = {};
    colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorImageView.subresourceRange.baseMipLevel = 0;
    colorImageView.subresourceRange.levelCount = 1;
    colorImageView.subresourceRange.baseArrayLayer = 0;
    colorImageView.subresourceRange.layerCount = 1;

    VkMemoryRequirements memReqs;

    VK_CHECK_RESULT(vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_color.image));
    vkGetImageMemoryRequirements(m_device, m_color.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_color.memory));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_color.image, m_color.memory, 0));

    VkCommandBuffer layoutCmd = m_vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

    vks::tools::setImageLayout(
            layoutCmd,
            m_color.image,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    colorImageView.image = m_color.image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &colorImageView, nullptr, &m_color.view));

    VkFormat fbDepthFormat;
    VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(m_vulkanDevice->physicalDevice, &fbDepthFormat);
    assert(validDepthFormat);
    // Depth stencil attachment
    imageCreateInfo.format = fbDepthFormat;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = fbDepthFormat;
    depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;

    VK_CHECK_RESULT(vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_depth.image));
    vkGetImageMemoryRequirements(m_device, m_depth.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_depth.memory));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depth.image, m_depth.memory, 0));

    vks::tools::setImageLayout(
            layoutCmd,
            m_depth.image,
            VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    m_vulkanDevice->flushCommandBuffer(layoutCmd, m_context->queue, true);

    depthStencilView.image = m_depth.image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depth.view));

    VkImageView attachments[2];
    attachments[0] = m_color.view;
    attachments[1] = m_depth.view;

    m_renderPass = new VulkanRenderPass();
    m_renderPass->setDevice(m_device);
    m_renderPass->setFormat(m_format);
    m_renderPass->setDepthFormat(fbDepthFormat);
    m_renderPass->createColorDepthPass();

    VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
    fbufCreateInfo.renderPass = m_renderPass->get();
    fbufCreateInfo.attachmentCount = 2;
    fbufCreateInfo.pAttachments = attachments;
    fbufCreateInfo.width = m_width;
    fbufCreateInfo.height = m_height;
    fbufCreateInfo.layers = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &fbufCreateInfo, nullptr, &m_frameBuffer));
}

END_NAMESPACE(VulkanEngine)