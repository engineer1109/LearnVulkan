#ifndef VULKAN_BASICENGINE_H
#define VULKAN_BASICENGINE_H
#include "vulkanexamplebase.h"
#include "vulkan_basicengine_help.h"
class VulkanBasicEngine:public VulkanExampleBase{
public:
    VulkanBasicEngine(bool enableValidation);
    ~VulkanBasicEngine();

    virtual void initWindow();

    void* getWindowPtr(){return m_window;}
protected:

private:
    void* m_window=nullptr;
};
#endif
