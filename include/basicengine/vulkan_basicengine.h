#ifndef VULKAN_BASICENGINE_H
#define VULKAN_BASICENGINE_H
#include "vulkanexamplebase.h"
#include "vulkan_basicengine_help.h"
#include "vulkan_basicengine_def.h"

class VK_BASICENGINE_EXPORT VulkanBasicEngine:public VulkanExampleBase{
public:
    VulkanBasicEngine(bool enableValidation);
    ~VulkanBasicEngine();
    
    std::string getWindowTitle();
    virtual void initWindow();
#ifdef __unix__
    virtual void setWindow(uint32_t window,xcb_connection_t* connection);
#elif WIN32
    virtual void setWindow(uint32_t window);
#endif
    virtual void renderLoop();

    void* getWindowPtr(){return m_window;}
    void setWindowFrameVisible(bool value){m_enableWindow=value;}

    std::string getShaderPath();

    void quitRender();
protected:

private:
    void* m_window=nullptr;
    bool m_enableWindow=true;
    bool m_quit=false;
};
#endif
