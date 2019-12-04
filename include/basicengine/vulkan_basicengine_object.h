/*
* LearnVulkan BasicEngine
*
* Copyright (C) by Jialiang Wang - https://github.com/engineer1109/LearnVulkan
* 1292846099@qq.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef VULKAN_BASICENGINE_OBJECT_H
#define VULKAN_BASICENGINE_OBJECT_H
#include "vulkan_basicengine_def.h"
#include "vulkan_basicengine_help.h"
#include "VulkanDevice.hpp"
namespace VulkanTemplate {

class VK_BASICENGINE_EXPORT VulkanBaseObject{
public:
    VulkanBaseObject()=default;
    ~VulkanBaseObject()=default;
public:
    struct ObjectInfo{
        vks::VulkanDevice* vulkanDevice=nullptr;
        VkInstance instance=VK_NULL_HANDLE;
        VkCommandPool cmdPool=VK_NULL_HANDLE;
        VkPipelineCache pipelineCache=VK_NULL_HANDLE;
        VkRenderPass renderPass=VK_NULL_HANDLE;
        VkQueue queue=VK_NULL_HANDLE;
        uint32_t* screenWitdh=nullptr;
        uint32_t* screenHeight=nullptr;
    }m_objectInfo;
public:
    void setObjectInfo(ObjectInfo info){
        this->m_vulkanDevice=info.vulkanDevice;
        this->m_device=this->m_vulkanDevice->logicalDevice;
        this->m_physicalDevice=this->m_vulkanDevice->physicalDevice;
        this->m_instance=info.instance;
        this->m_cmdPool=info.cmdPool;
        this->m_pipelineCache=info.pipelineCache;
        this->m_renderPass=info.renderPass;
        this->m_queue=info.queue;
        this->m_screenWidth=info.screenWitdh;
        this->m_screenHeight=info.screenHeight;
    }
    void setPipelineLayout(VkPipelineLayout pipelineLayout){m_pipelineLayout=pipelineLayout;}

protected:
    vks::VulkanDevice* m_vulkanDevice=nullptr;
    VkDevice m_device=VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice=VK_NULL_HANDLE;
    VkInstance m_instance=VK_NULL_HANDLE;
    VkCommandPool m_cmdPool=VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout=nullptr;
    VkPipelineCache m_pipelineCache=VK_NULL_HANDLE;
    VkRenderPass m_renderPass=VK_NULL_HANDLE;
    VkQueue m_queue=VK_NULL_HANDLE;
    uint32_t* m_screenWidth=nullptr;
    uint32_t* m_screenHeight=nullptr;
};

}
#endif // VULKAN_BASICENGINE_OBJECT_H
