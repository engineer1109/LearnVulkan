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
    vkDestroySampler(m_device, m_depthSampler, nullptr);

    // Depth attachment
    vkDestroyImageView(m_device, m_view, nullptr);
    vkDestroyImage(m_device, m_image, nullptr);
    vkFreeMemory(m_device, m_memory, nullptr);

    vkDestroyFramebuffer(m_device, m_frameBuffer, nullptr);
}

void VulkanFrameBuffer::create() {
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
    VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_image));

    VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(m_device, m_image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = m_vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_memory));
    VK_CHECK_RESULT(vkBindImageMemory(m_device, m_image, m_memory, 0));

    VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = m_format;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = m_image;
    VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_view));

    // Create sampler to sample from to depth attachment
    // Used to sample in the fragment shader for shadowed rendering
    VkFilter shadowmap_filter = VK_FILTER_LINEAR;
    VkSamplerCreateInfo sampler = vks::initializers::samplerCreateInfo();
    sampler.magFilter = shadowmap_filter;
    sampler.minFilter = shadowmap_filter;
    sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
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
    m_renderPass->create();

    // Create frame buffer
    VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
    fbufCreateInfo.renderPass = m_renderPass->get();
    fbufCreateInfo.attachmentCount = 1;
    fbufCreateInfo.pAttachments = &m_view;
    fbufCreateInfo.width = m_width;
    fbufCreateInfo.height = m_height;
    fbufCreateInfo.layers = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &fbufCreateInfo, nullptr, &m_frameBuffer));

    m_descriptor = vks::initializers::descriptorImageInfo(
            m_depthSampler,
            m_view,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
}

END_NAMESPACE(VulkanEngine)