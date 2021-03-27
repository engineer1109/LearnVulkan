//
// Created by wjl on 21-3-28.
//

#include "VulkanTextureCubeMap.h"

#include "AssetReader.h"

#include <stb_image_aug.h>

BEGIN_NAMESPACE(VulkanEngine)

void VulkanTextureCubeMap::loadFromFile(std::vector<std::string> files, AAssetManager *asset,
                                        VkFormat format, vks::VulkanDevice *device,
                                        VkQueue copyQueue, VkImageUsageFlags imageUsageFlags,
                                        VkImageLayout imageLayout, bool forceLinear) {

    std::vector<AssetReader *> assetReaders(files.size());

    for (size_t i = 0; i < files.size(); i++) {
        assetReaders[i] = new AssetReader(files[i].c_str(), asset);
        assetReaders[i]->run();
    }

    int w, h, c = 0;
    std::vector<uint8_t *> imgList(files.size());

    for (size_t i = 0; i < files.size(); i++) {
        imgList[i] = stbi_load_from_memory((uint8_t *) assetReaders[i]->getOutData(),
                                           assetReaders[i]->getSize(), &w, &h, &c, 0);
    }

    this->device = device;
    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
    mipLevels = static_cast<uint32_t>(1);
    channels = static_cast<uint32_t>(c);

    m_size = width * height * channels * files.size();
    uint8_t *img = new uint8_t[m_size];

    for (int i = 0; i < files.size(); i++) {
        memcpy(img + i * width * height * channels, imgList[i], width * height * channels);
    }

    VkMemoryAllocateInfo memAllocInfo = vks::initializers::memoryAllocateInfo();
    VkMemoryRequirements memReqs;

    // Create a host-visible staging buffer that contains the raw image data
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;

    VkBufferCreateInfo bufferCreateInfo = vks::initializers::bufferCreateInfo();
    bufferCreateInfo.size = m_size;
    // This buffer is used as a transfer source for the buffer copy
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK_RESULT(
            vkCreateBuffer(device->logicalDevice, &bufferCreateInfo, nullptr, &stagingBuffer));

    // Get memory requirements for the staging buffer (alignment, memory type bits)
    vkGetBufferMemoryRequirements(device->logicalDevice, stagingBuffer, &memReqs);

    memAllocInfo.allocationSize = memReqs.size;
    // Get memory type index for a host visible buffer
    memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VK_CHECK_RESULT(
            vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &stagingMemory));
    VK_CHECK_RESULT(vkBindBufferMemory(device->logicalDevice, stagingBuffer, stagingMemory, 0));

    // Copy texture data into staging buffer
    uint8_t *data;
    VK_CHECK_RESULT(
            vkMapMemory(device->logicalDevice, stagingMemory, 0, memReqs.size, 0, (void **) &data));
    memcpy(data, img, m_size);
    vkUnmapMemory(device->logicalDevice, stagingMemory);

    // Setup buffer copy regions for each face including all of it's miplevels
    std::vector<VkBufferImageCopy> bufferCopyRegions;
    size_t offset = 0;

    for (uint32_t face = 0; face < 6; face++) {
        for (uint32_t level = 0; level < mipLevels; level++) {
            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = face;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(width);
            bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(height);
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = offset;

            bufferCopyRegions.push_back(bufferCopyRegion);

            // Increase offset into staging buffer for next level / face
            offset += width * height * channels;
        }
    }

    // Create optimal tiled target image
    VkImageCreateInfo imageCreateInfo = vks::initializers::imageCreateInfo();
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = format;
    imageCreateInfo.mipLevels = mipLevels;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.extent = {width, height, 1};
    imageCreateInfo.usage = imageUsageFlags;
    // Ensure that the TRANSFER_DST bit is set for staging
    if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
        imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    // Cube faces count as array layers in Vulkan
    imageCreateInfo.arrayLayers = 6;
    // This flag is required for cube map images
    imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;


    VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &imageCreateInfo, nullptr, &image));

    vkGetImageMemoryRequirements(device->logicalDevice, image, &memReqs);

    memAllocInfo.allocationSize = memReqs.size;
    memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits,
                                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &deviceMemory));
    VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, image, deviceMemory, 0));

    // Use a separate command buffer for texture loading
    VkCommandBuffer copyCmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

    // Image barrier for optimal image (target)
    // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.layerCount = 6;

    vks::tools::setImageLayout(
            copyCmd,
            image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange);

    // Copy the cube map faces from the staging buffer to the optimal tiled image
    vkCmdCopyBufferToImage(
            copyCmd,
            stagingBuffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            static_cast<uint32_t>(bufferCopyRegions.size()),
            bufferCopyRegions.data());

    // Change texture image layout to shader read after all faces have been copied
    this->imageLayout = imageLayout;
    vks::tools::setImageLayout(
            copyCmd,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            imageLayout,
            subresourceRange);

    device->flushCommandBuffer(copyCmd, copyQueue);

    // Create sampler
    VkSamplerCreateInfo samplerCreateInfo = vks::initializers::samplerCreateInfo();
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
    samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.maxAnisotropy = device->enabledFeatures.samplerAnisotropy
                                      ? device->properties.limits.maxSamplerAnisotropy : 1.0f;
    samplerCreateInfo.anisotropyEnable = device->enabledFeatures.samplerAnisotropy;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = (float) mipLevels;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VK_CHECK_RESULT(vkCreateSampler(device->logicalDevice, &samplerCreateInfo, nullptr, &sampler));

    // Create image view
    VkImageViewCreateInfo viewCreateInfo = vks::initializers::imageViewCreateInfo();
    viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
    viewCreateInfo.format = format;
    viewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
                                 VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
    viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    viewCreateInfo.subresourceRange.layerCount = 6;
    viewCreateInfo.subresourceRange.levelCount = mipLevels;
    viewCreateInfo.image = image;
    VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &viewCreateInfo, nullptr, &view));

    // Clean up staging resources
    vkFreeMemory(device->logicalDevice, stagingMemory, nullptr);
    vkDestroyBuffer(device->logicalDevice, stagingBuffer, nullptr);

    // Update descriptor image info member that can be used for setting up descriptor sets
    updateDescriptor();

    delete_array(img);
    for(auto &assetReader:assetReaders){
        delete_ptr(assetReader);
    }
    for (size_t i = 0; i < imgList.size(); i++) {
        stbi_image_free(imgList[i]);
    }
}

END_NAMESPACE(VulkanEngine)
