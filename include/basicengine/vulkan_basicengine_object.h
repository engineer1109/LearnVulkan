/*
* LearnVulkan BasicEngine
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef VULKAN_BASICENGINE_OBJECT_H
#define VULKAN_BASICENGINE_OBJECT_H
#include "vulkan_basicengine_def.h"
#include "vulkan_basicengine_help.h"
#include "VulkanDevice.hpp"
BEGIN_NAMESPACE(VulkanTemplate)

class VK_BASICENGINE_EXPORT VulkanBaseObject{
public:
    VulkanBaseObject()=default;
    ~VulkanBaseObject()=default;
public:
    struct ObjectInfo{
        vks::VulkanDevice* vulkanDevice=nullptr;
        VkInstance instance=VK_NULL_HANDLE;
        VkCommandPool cmdPool=VK_NULL_HANDLE;
        VkPipelineLayout* pPipelineLayout=nullptr;
        VkPipelineCache* pPipelineCache=VK_NULL_HANDLE;
        VkRenderPass* pRenderPass=nullptr;
        VkQueue queue=nullptr;
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
        this->m_pPipelineLayout=info.pPipelineLayout;
        this->m_pPipelineCache=info.pPipelineCache;
        this->m_pRenderPass=info.pRenderPass;
        this->m_queue=info.queue;
        this->m_screenWidth=info.screenWitdh;
        this->m_screenHeight=info.screenHeight;
    }

protected:
    vks::VulkanDevice* m_vulkanDevice=nullptr;
    VkDevice m_device=VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice=VK_NULL_HANDLE;
    VkInstance m_instance=VK_NULL_HANDLE;
    VkCommandPool m_cmdPool=VK_NULL_HANDLE;
    VkPipelineLayout* m_pPipelineLayout=nullptr;
    VkPipelineCache* m_pPipelineCache=nullptr;
    VkRenderPass* m_pRenderPass=nullptr;
    VkQueue m_queue=nullptr;
    uint32_t* m_screenWidth=nullptr;
    uint32_t* m_screenHeight=nullptr;
};

END_NAMESPACE(VulkanTemplate)
#endif // VULKAN_BASICENGINE_OBJECT_H
