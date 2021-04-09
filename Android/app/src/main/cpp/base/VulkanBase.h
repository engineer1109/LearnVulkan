//
// Created by wjl on 21-2-17.
//

#ifndef LEARNVULKAN_VULKANBASE_H
#define LEARNVULKAN_VULKANBASE_H

#include "render_common.h"
#include "base_template.h"
#include "vulkan_macro.h"
#include "vulkan_template.h"

#include "VulkanTools.h"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BEGIN_NAMESPACE(VulkanEngine)

class VulkanBase {
public:
    enum class TouchMode{
        NONE = 0,
        SINGLE = 1,
        DOUBLE = 2,
    };
public:
    VulkanBase() = default;

    virtual ~VulkanBase();

    void setWindow(ANativeWindow *window) { m_window = window;}

    void setWidth(const uint32_t &width) { m_width = width;}

    void setHeight(const uint32_t &height) { m_height = height; }

    void setAssetManager(AAssetManager *asset) { m_asset = asset; }

    void initVulkan();

    virtual void prepare();

    virtual void prepareBase();

    virtual void renderLoop();

    virtual void renderFrame();

    virtual void render();

    virtual void draw();

    virtual void defaultTouchOperation();

    void waitForCurrentFrameComplete();

    void setTouchPos(const float &x, const float &y){
        m_mousePos[0] = {x, y};
    }

    void setTouchPosSecond(const float &x, const float &y){
        m_mousePos[1] = {x, y};
    }

    void setTouchMode(TouchMode mode){
        m_touchMode = mode;
    }

    void resetTouch(){
        m_mousePos[0] = {0.f,0.f};
        m_mousePos[1] = {0.f,0.f};
        m_mousePosOld[0] = {0.f,0.f};
        m_mousePosOld[1] = {0.f,0.f};
        m_touchMode = TouchMode::NONE;
    }

    void pause(){
        m_pause = true;
    }

    void resume(){
        m_pause = false;
    }

protected:
    void createInstance();

    void pickPhysicalDevice();

    virtual void getDeviceFeatures() {}

    void createLogicalDevice();

    void initSwapchain();

    void createCommandPool();

    void setupSwapChain();

    void createCommandBuffers();

    void createSynchronizationPrimitives();

    void setupDepthStencil();

    void setupRenderPass();

    void createPipelineCache();

    void setupFrameBuffer();

    virtual void buildCommandBuffers(){};

    void prepareFrame();

    void submitFrame();

public:

    void destroySurface();

    void destroyCommandBuffers();

    void rebuildSurface();

    void destroyANativeWindow();

    virtual void prepareFunctions() {}

    virtual void runFunction(int i);

protected:
    ANativeWindow *m_window = nullptr;
    AAssetManager *m_asset = nullptr;
    uint32_t m_width = 1280;
    uint32_t m_height = 720;

    bool m_debug = false;

    VkResult m_result = VK_SUCCESS;
    VkInstance m_instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_queue = VK_NULL_HANDLE;
    VkCommandPool m_cmdPool = VK_NULL_HANDLE;

    std::vector<const char *> m_enabledDeviceExtensions;
    std::vector<const char *> m_enabledInstanceExtensions;
    VkPhysicalDeviceFeatures m_enabledFeatures;
    void *m_deviceCreatepNextChain = nullptr;

    VkPhysicalDeviceProperties m_deviceProperties;
    VkPhysicalDeviceFeatures m_deviceFeatures;
    VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;

    vks::VulkanDevice *m_vulkanDevice = nullptr;
    VulkanSwapChain m_swapChain;
    struct RenderSemaphores {
        // Swap chain image presentation
        VkSemaphore presentComplete = VK_NULL_HANDLE;
        // Command buffer submission and execution
        VkSemaphore renderComplete = VK_NULL_HANDLE;
    } m_semaphores;

    struct {
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory mem = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
    } m_depthStencil;

    std::vector<VkFence> m_waitFences;

    VkFormat m_depthFormat = VK_FORMAT_D16_UNORM_S8_UINT;
    VkSubmitInfo m_submitInfo;
    VkPipelineStageFlags m_submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    //Render Context
    std::vector<VkCommandBuffer> m_drawCmdBuffers;
    VkRenderPass m_renderPass;
    std::vector<VkFramebuffer> m_frameBuffers;
    uint32_t m_currentBuffer = 0;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
    std::vector<VkShaderModule> m_shaderModules;
    VkPipelineCache m_pipelineCache;

    bool m_stop = false;
    bool m_quit = false;
    bool m_pause = false;
    bool m_prepared = false;
    bool m_signalFrame = true;

    glm::vec2 m_mousePos[2];
    glm::vec2 m_mousePosOld[2];
    TouchMode m_touchMode = TouchMode::NONE;

    float m_distance = 0.f;
    float m_oldDistance = 0.f;

    std::vector<std::function<void()>> m_functions;
};

END_NAMESPACE(VulkanEngine)


#endif //LEARNVULKAN_VULKANBASE_H
