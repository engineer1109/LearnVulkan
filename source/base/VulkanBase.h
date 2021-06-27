//
// Created by wjl on 21-2-17.
//

#ifndef LEARNVULKAN_VULKANBASE_H
#define LEARNVULKAN_VULKANBASE_H

#include "render_common.h"
#include "base_template.h"
#include "filesystem_utils.h"
#include "vulkan_macro.h"
#include "vulkan_template.h"

#include "VulkanTools.h"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "keycodes.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanBase {
public:
    VulkanBase() = default;

    virtual ~VulkanBase();

    void setWidth(const uint32_t &width) { m_width = width; }

    void setHeight(const uint32_t &height) { m_height = height; }

    void initVulkan();

    virtual void prepare();

    virtual void prepareBase();

    virtual void renderLoop();

    virtual void renderFrame();

    virtual void updateOverlay() {}

    virtual void render();

    virtual void draw();

    virtual void updateCommand() {}

    void waitForCurrentFrameComplete();

    void pause() { m_pause = true; }

    void resume() { m_pause = false; }

    void quit() { m_quit = true; }

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

    virtual void buildCommandBuffers() {};

    void prepareFrame();

    void submitFrame();

public:

    void destroySurface();

    void destroyCommandBuffers();

    void windowResize();

    void handleMouseMove(float x, float y);

    virtual void keyPressed(uint32_t) {}

    virtual void prepareFunctions() {}

    virtual void runFunction(int i);

#if defined(_WIN32)
    HWND setupWindow();
    void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool m_resizing = false;
#elif defined(VK_USE_PLATFORM_XCB_KHR)

    void initxcbConnection();

    xcb_window_t setupWindow();

    void handleEvent(const xcb_generic_event_t *event);

#endif

    void setMouseButtonLeft(bool value) { m_mouseButtons.left = value; }

    void setMouseButtonRight(bool value) { m_mouseButtons.right = value; }

    void setMouseButtonMiddle(bool value) { m_mouseButtons.middle = value; }

#if defined(_WIN32)
    void setWindow(HWND window) {
#elif defined(VK_USE_PLATFORM_XCB_KHR)

    void setWindow(xcb_window_t window) {
        m_window = window;
    }

#endif

protected:
#if defined(_WIN32)
    HWND m_window = NULL;
    HINSTANCE m_windowInstance = NULL;
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    xcb_connection_t *m_connection = nullptr;
    xcb_screen_t *m_screen = nullptr;
    xcb_window_t m_window = NULL;
    xcb_intern_atom_reply_t *m_atom_wm_delete_window = nullptr;
#endif
    uint32_t m_width = 1280;
    uint32_t m_height = 720;
    uint32_t m_destWidth = 1280;
    uint32_t m_destHeight = 720;

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

    glm::vec2 m_mousePos;
    glm::vec2 m_mousePosOld;

    float m_distance = 0.f;
    float m_oldDistance = 0.f;

    struct Settings {
        bool fullScreen = false;
    } m_settings;

    struct MouseButton {
        bool left = false;
        bool right = false;
        bool middle = false;
    } m_mouseButtons;

    struct Scroll {
        bool up = false;
        bool down = false;
    } m_scroll;

    std::string m_title = "Vulkan";

    std::vector<std::function<void()>> m_functions;

    float m_frameTimer = 0.f;
};

END_NAMESPACE(VulkanEngine)


#endif //LEARNVULKAN_VULKANBASE_H
