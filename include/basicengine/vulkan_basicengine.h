/*
* LearnVulkan BasicEngine
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef VULKAN_BASICENGINE_H
#define VULKAN_BASICENGINE_H
#include "vulkanexamplebase.h"
#include "vulkan_basicengine_help.h"
#include "vulkan_basicengine_filesystem.h"
#include "vulkan_basicengine_def.h"

class VK_BASICENGINE_EXPORT VulkanBasicEngine:public VulkanExampleBase{
public:
    VulkanBasicEngine(bool enableValidation);
    ~VulkanBasicEngine();
    
    std::string getWindowTitle();
    virtual void initWindow();
    virtual void setWindow(uint32_t window);
    virtual void renderLoop();

    virtual void renderAsyncThread();
    virtual void renderJoin();

    void* getWindowPtr(){return m_window;}
    void setWindowFrameVisible(bool value){m_enableWindow=value;}

    std::string getShaderPath();

    void quitRender();
    void preRender();
    void singleRender();
    void afterRender();

    void handleMouse(int x,int y);
protected:
    VkPipelineLayout m_pipelineLayout=VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout=VK_NULL_HANDLE;
private:
    void* m_window=nullptr;
    std::thread* m_thread=nullptr;
    bool m_enableWindow=true;
    bool m_quit=false;
};
#endif
