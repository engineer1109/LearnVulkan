//
// Created by wjl on 21-2-17.
//

#ifndef LEARNVULKAN_VULKANBASE_H
#define LEARNVULKAN_VULKANBASE_H

#include "render_common.h"
#include "VulkanTools.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BEGIN_NAMESPACE(LearnVulkan)

class VulkanBase {
public:
    enum class TouchMode{
        NONE = 0,
        SINGLE = 1,
        DOUBLE = 2,
    };
public:
    VulkanBase();

    ~VulkanBase();

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

    virtual void defaultTouchOperation();

protected:
    void createInstance();

    void pickPhysicalDevice();

    virtual void getDeviceFeatures() = 0;

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

    VkPhysicalDeviceProperties m_deviceProperties;
    VkPhysicalDeviceFeatures m_deviceFeatures;
    VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;
};

END_NAMESPACE(LearnVulkan)


#endif //LEARNVULKAN_VULKANBASE_H
