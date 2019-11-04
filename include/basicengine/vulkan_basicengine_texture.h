#ifndef VULKAN_BASICENGINE_TEXTURE_H
#define VULKAN_BASICENGINE_TEXTURE_H
#ifdef WIN32
    #define gli glm
#endif
#include <iostream>
#include "VulkanTexture.hpp"
#include "stb_image_aug.h"
namespace vks {

struct Texture2DStbImage: public Texture2D{
    uint32_t channels=0;
    uint32_t size=0;
    void loadFromFile(
        std::string filename,
        VkFormat format,
        vks::VulkanDevice *device,
        VkQueue copyQueue,
        VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        bool forceLinear = false)
    {
        if (!vks::tools::fileExists(filename)) {
            vks::tools::exitFatal("Could not load texture from " + filename + "\n\nThe file may be part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
        }
        int w,h,n;
        unsigned char *img = stbi_load(filename.c_str(), &w, &h, &n, 0);

        this->device = device;
        width = static_cast<uint32_t>(w);
        height = static_cast<uint32_t>(h);
        mipLevels = static_cast<uint32_t>(1);
        channels=static_cast<uint32_t>(n);
        size=width*height*channels;

        // Get device properites for the requested texture format
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(device->physicalDevice, format, &formatProperties);

        // Only use linear tiling if requested (and supported by the device)
        // Support for linear tiling is mostly limited, so prefer to use
        // optimal tiling instead
        // On most implementations linear tiling will only support a very
        // limited amount of formats and features (mip maps, cubemaps, arrays, etc.)
        VkBool32 useStaging = !forceLinear;

        VkMemoryAllocateInfo memAllocInfo = vks::initializers::memoryAllocateInfo();
        VkMemoryRequirements memReqs;

        // Use a separate command buffer for texture loading
        VkCommandBuffer copyCmd = device->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

        if (useStaging)
        {
            // Create a host-visible staging buffer that contains the raw image data
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingMemory;

            VkBufferCreateInfo bufferCreateInfo = vks::initializers::bufferCreateInfo();
            bufferCreateInfo.size = width*height*channels;
            // This buffer is used as a transfer source for the buffer copy
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VK_CHECK_RESULT(vkCreateBuffer(device->logicalDevice, &bufferCreateInfo, nullptr, &stagingBuffer));

            // Get memory requirements for the staging buffer (alignment, memory type bits)
            vkGetBufferMemoryRequirements(device->logicalDevice, stagingBuffer, &memReqs);

            memAllocInfo.allocationSize = memReqs.size;
            // Get memory type index for a host visible buffer
            memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &stagingMemory));
            VK_CHECK_RESULT(vkBindBufferMemory(device->logicalDevice, stagingBuffer, stagingMemory, 0));

            // Copy texture data into staging buffer
            uint8_t *data;
            VK_CHECK_RESULT(vkMapMemory(device->logicalDevice, stagingMemory, 0, memReqs.size, 0, (void **)&data));
            memcpy(data, img, size);
            vkUnmapMemory(device->logicalDevice, stagingMemory);

            // Setup buffer copy regions for each mip level
            std::vector<VkBufferImageCopy> bufferCopyRegions;
            uint32_t offset = 0;

            for (uint32_t i = 0; i < mipLevels; i++)
            {
                VkBufferImageCopy bufferCopyRegion = {};
                bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                bufferCopyRegion.imageSubresource.mipLevel = i;
                bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
                bufferCopyRegion.imageSubresource.layerCount = 1;
                bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(width);
                bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(height);
                bufferCopyRegion.imageExtent.depth = 1;
                bufferCopyRegion.bufferOffset = offset;

                bufferCopyRegions.push_back(bufferCopyRegion);

                offset += static_cast<uint32_t>(size);
            }

            // Create optimal tiled target image
            VkImageCreateInfo imageCreateInfo = vks::initializers::imageCreateInfo();
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = format;
            imageCreateInfo.mipLevels = mipLevels;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageCreateInfo.extent = { width, height, 1 };
            imageCreateInfo.usage = imageUsageFlags;
            // Ensure that the TRANSFER_DST bit is set for staging
            if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
            {
                imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            }
            VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &imageCreateInfo, nullptr, &image));

            vkGetImageMemoryRequirements(device->logicalDevice, image, &memReqs);

            memAllocInfo.allocationSize = memReqs.size;

            memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &deviceMemory));
            VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, image, deviceMemory, 0));

            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = mipLevels;
            subresourceRange.layerCount = 1;

            // Image barrier for optimal image (target)
            // Optimal image will be used as destination for the copy
            vks::tools::setImageLayout(
                copyCmd,
                image,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                subresourceRange);

            // Copy mip levels from staging buffer
            vkCmdCopyBufferToImage(
                copyCmd,
                stagingBuffer,
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                static_cast<uint32_t>(bufferCopyRegions.size()),
                bufferCopyRegions.data()
            );

            // Change texture image layout to shader read after all mip levels have been copied
            this->imageLayout = imageLayout;
            vks::tools::setImageLayout(
                copyCmd,
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                imageLayout,
                subresourceRange);

            device->flushCommandBuffer(copyCmd, copyQueue);

            // Clean up staging resources
            vkFreeMemory(device->logicalDevice, stagingMemory, nullptr);
            vkDestroyBuffer(device->logicalDevice, stagingBuffer, nullptr);
        }
        else
        {
            // Prefer using optimal tiling, as linear tiling
            // may support only a small set of features
            // depending on implementation (e.g. no mip maps, only one layer, etc.)

            // Check if this support is supported for linear tiling
            assert(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

            VkImage mappableImage;
            VkDeviceMemory mappableMemory;

            VkImageCreateInfo imageCreateInfo = vks::initializers::imageCreateInfo();
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = format;
            imageCreateInfo.extent = { width, height, 1 };
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
            imageCreateInfo.usage = imageUsageFlags;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

            // Load mip map level 0 to linear tiling image
            VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &imageCreateInfo, nullptr, &mappableImage));

            // Get memory requirements for this image
            // like size and alignment
            vkGetImageMemoryRequirements(device->logicalDevice, mappableImage, &memReqs);
            // Set memory allocation size to required memory size
            memAllocInfo.allocationSize = memReqs.size;

            // Get memory type that can be mapped to host memory
            memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            // Allocate host memory
            VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &mappableMemory));

            // Bind allocated image for use
            VK_CHECK_RESULT(vkBindImageMemory(device->logicalDevice, mappableImage, mappableMemory, 0));

            // Get sub resource layout
            // Mip map count, array layer, etc.
            VkImageSubresource subRes = {};
            subRes.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subRes.mipLevel = 0;

            VkSubresourceLayout subResLayout;
            void *data;

            // Get sub resources layout
            // Includes row pitch, size offsets, etc.
            vkGetImageSubresourceLayout(device->logicalDevice, mappableImage, &subRes, &subResLayout);

            // Map image memory
            VK_CHECK_RESULT(vkMapMemory(device->logicalDevice, mappableMemory, 0, memReqs.size, 0, &data));

            // Copy image data into memory
            memcpy(data, img, size);

            vkUnmapMemory(device->logicalDevice, mappableMemory);

            // Linear tiled images don't need to be staged
            // and can be directly used as textures
            image = mappableImage;
            deviceMemory = mappableMemory;
            this->imageLayout = imageLayout;

            // Setup image memory barrier
            vks::tools::setImageLayout(copyCmd, image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, imageLayout);

            device->flushCommandBuffer(copyCmd, copyQueue);
        }

        // Create a defaultsampler
        VkSamplerCreateInfo samplerCreateInfo = {};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
        samplerCreateInfo.minLod = 0.0f;
        // Max level-of-detail should match mip level count
        samplerCreateInfo.maxLod = (useStaging) ? (float)mipLevels : 0.0f;
        // Only enable anisotropic filtering if enabled on the devicec
        samplerCreateInfo.maxAnisotropy = device->enabledFeatures.samplerAnisotropy ? device->properties.limits.maxSamplerAnisotropy : 1.0f;
        samplerCreateInfo.anisotropyEnable = device->enabledFeatures.samplerAnisotropy;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        VK_CHECK_RESULT(vkCreateSampler(device->logicalDevice, &samplerCreateInfo, nullptr, &sampler));

        // Create image view
        // Textures are not directly accessed by the shaders and
        // are abstracted by image views containing additional
        // information and sub resource ranges
        VkImageViewCreateInfo viewCreateInfo = {};
        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewCreateInfo.format = format;
        viewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        viewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        // Linear tiling usually won't support mip maps
        // Only set mip map count if optimal tiling is used
        viewCreateInfo.subresourceRange.levelCount = (useStaging) ? mipLevels : 1;
        viewCreateInfo.image = image;
        VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &viewCreateInfo, nullptr, &view));

        // Update descriptor image info member that can be used for setting up descriptor sets
        updateDescriptor();
        stbi_image_free(img);
    }

};

class TextureCubeMapStbImage:public TextureCubeMap{
public:
    uint32_t channels=0;
    uint32_t size=0;
    void loadFromFile(
        std::vector<std::string> filenameList,
        VkFormat format,
        vks::VulkanDevice *device,
        VkQueue copyQueue,
        VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT,
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        for(size_t i=0;i<filenameList.size();i++){
            std::string filename=filenameList[i];
            if (!vks::tools::fileExists(filename)) {
                vks::tools::exitFatal("Could not load texture from " + filename + "\n\nThe file may be part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
            }
        }
        int w,h,n;
        std::vector<uint8_t*> imgList(filenameList.size());

        for (size_t i=0;i<imgList.size();i++) {
            imgList[i]=stbi_load(filenameList[i].c_str(), &w, &h, &n, 0);
        }

        this->device = device;
        width = static_cast<uint32_t>(w);
        height = static_cast<uint32_t>(h);
        mipLevels = static_cast<uint32_t>(1);
        channels=static_cast<uint32_t>(n);
        size=width*height*4*filenameList.size();
        uint32_t imgSize=width*height*4;
        uint8_t* img=new uint8_t[size];
        uint8_t* imgT=new uint8_t[size];

        if(n==4){
            for (size_t i=0;i<imgList.size();i++) {
                memcpy(img+i*width*height*channels,imgList[i],width*height*channels*sizeof(uint8_t));
                stbi_image_free(imgList[i]);
            }
        }
        // Turn RGB to RGBA
        else if(n==3){
            #pragma omp parallel for
            for (int i=0;i<imgList.size();i++) {
                for(uint32_t m=0;m<height;m++){
                    for (uint32_t n=0;n<width;n++) {
                        img[imgSize*i+(n+width*m)*4+0]=imgList[i][(n+width*m)*3+0];
                        img[imgSize*i+(n+width*m)*4+1]=imgList[i][(n+width*m)*3+1];
                        img[imgSize*i+(n+width*m)*4+2]=imgList[i][(n+width*m)*3+2];
                        img[imgSize*i+(n+width*m)*4+3]=255;
                    }
                }
                stbi_image_free(imgList[i]);
            }
        }
        #pragma omp parallel for
        for (int i=0;i<imgList.size();i++) {
            if(i==2){
                for(uint32_t m=0;m<height;m++){
                    for (uint32_t n=0;n<width;n++) {
                        imgT[imgSize*i+(n+width*m)*4+0]=img[imgSize*i+(m+width*(n))*4+0];
                        imgT[imgSize*i+(n+width*m)*4+1]=img[imgSize*i+(m+width*(n))*4+1];
                        imgT[imgSize*i+(n+width*m)*4+2]=img[imgSize*i+(m+width*(n))*4+2];
                        imgT[imgSize*i+(n+width*m)*4+3]=img[imgSize*i+(m+width*(n))*4+3];
                    }
                }
            }
            else if (i==3){
                for(uint32_t m=0;m<height;m++){
                    for (uint32_t n=0;n<width;n++) {
                        imgT[imgSize*i+(n+width*m)*4+0]=img[imgSize*i+((height-1-m)+width*(width-1-n))*4+0];
                        imgT[imgSize*i+(n+width*m)*4+1]=img[imgSize*i+((height-1-m)+width*(width-1-n))*4+1];
                        imgT[imgSize*i+(n+width*m)*4+2]=img[imgSize*i+((height-1-m)+width*(width-1-n))*4+2];
                        imgT[imgSize*i+(n+width*m)*4+3]=img[imgSize*i+((height-1-m)+width*(width-1-n))*4+3];
                    }
                }
            }
            else {
                for(uint32_t m=0;m<height;m++){
                    for (uint32_t n=0;n<width;n++) {
                        imgT[imgSize*i+(n+width*m)*4+0]=img[imgSize*i+(n+width*(height-1-m))*4+0];
                        imgT[imgSize*i+(n+width*m)*4+1]=img[imgSize*i+(n+width*(height-1-m))*4+1];
                        imgT[imgSize*i+(n+width*m)*4+2]=img[imgSize*i+(n+width*(height-1-m))*4+2];
                        imgT[imgSize*i+(n+width*m)*4+3]=img[imgSize*i+(n+width*(height-1-m))*4+3];
                    }
                }
            }
        }


        VkMemoryAllocateInfo memAllocInfo = vks::initializers::memoryAllocateInfo();
        VkMemoryRequirements memReqs;

        // Create a host-visible staging buffer that contains the raw image data
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        VkBufferCreateInfo bufferCreateInfo = vks::initializers::bufferCreateInfo();
        bufferCreateInfo.size = size;
        // This buffer is used as a transfer source for the buffer copy
        bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK_RESULT(vkCreateBuffer(device->logicalDevice, &bufferCreateInfo, nullptr, &stagingBuffer));

        // Get memory requirements for the staging buffer (alignment, memory type bits)
        vkGetBufferMemoryRequirements(device->logicalDevice, stagingBuffer, &memReqs);

        memAllocInfo.allocationSize = memReqs.size;
        // Get memory type index for a host visible buffer
        memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        VK_CHECK_RESULT(vkAllocateMemory(device->logicalDevice, &memAllocInfo, nullptr, &stagingMemory));
        VK_CHECK_RESULT(vkBindBufferMemory(device->logicalDevice, stagingBuffer, stagingMemory, 0));

        // Copy texture data into staging buffer
        uint8_t *data;
        VK_CHECK_RESULT(vkMapMemory(device->logicalDevice, stagingMemory, 0, memReqs.size, 0, (void **)&data));
        memcpy(data, imgT, size);
        vkUnmapMemory(device->logicalDevice, stagingMemory);

        // Setup buffer copy regions for each face including all of it's miplevels
        std::vector<VkBufferImageCopy> bufferCopyRegions;
        size_t offset = 0;

        for (uint32_t face = 0; face < 6; face++)
        {
            for (uint32_t level = 0; level < mipLevels; level++)
            {
                VkBufferImageCopy bufferCopyRegion = {};
                bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                bufferCopyRegion.imageSubresource.mipLevel = level;
                bufferCopyRegion.imageSubresource.baseArrayLayer = face;
                bufferCopyRegion.imageSubresource.layerCount = 1;
                bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(w);
                bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(h);
                bufferCopyRegion.imageExtent.depth = 1;
                bufferCopyRegion.bufferOffset = offset;

                bufferCopyRegions.push_back(bufferCopyRegion);

                // Increase offset into staging buffer for next level / face
                offset += width*height*4;
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
        imageCreateInfo.extent = { width, height, 1 };
        imageCreateInfo.usage = imageUsageFlags;
        // Ensure that the TRANSFER_DST bit is set for staging
        if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
        {
            imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        // Cube faces count as array layers in Vulkan
        imageCreateInfo.arrayLayers = 6;
        // This flag is required for cube map images
        imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;


        VK_CHECK_RESULT(vkCreateImage(device->logicalDevice, &imageCreateInfo, nullptr, &image));

        vkGetImageMemoryRequirements(device->logicalDevice, image, &memReqs);

        memAllocInfo.allocationSize = memReqs.size;
        memAllocInfo.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

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
        samplerCreateInfo.maxAnisotropy = device->enabledFeatures.samplerAnisotropy ? device->properties.limits.maxSamplerAnisotropy : 1.0f;
        samplerCreateInfo.anisotropyEnable = device->enabledFeatures.samplerAnisotropy;
        samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = (float)mipLevels;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        VK_CHECK_RESULT(vkCreateSampler(device->logicalDevice, &samplerCreateInfo, nullptr, &sampler));

        // Create image view
        VkImageViewCreateInfo viewCreateInfo = vks::initializers::imageViewCreateInfo();
        viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        viewCreateInfo.format = format;
        viewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        viewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        viewCreateInfo.subresourceRange.layerCount = 6;
        viewCreateInfo.subresourceRange.levelCount = mipLevels;
        viewCreateInfo.image = image;
        VK_CHECK_RESULT(vkCreateImageView(device->logicalDevice, &viewCreateInfo, nullptr, &view));

        // Clean up staging resources
        vkFreeMemory(device->logicalDevice, stagingMemory, nullptr);
        vkDestroyBuffer(device->logicalDevice, stagingBuffer, nullptr);

        // Update descriptor image info member that can be used for setting up descriptor sets
        updateDescriptor();

        delete[] img;
        delete[] imgT;
    }
};

}


#endif // VULKAN_BASICENGINE_TEXTURE_H
