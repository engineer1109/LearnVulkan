//
// Created by wjl on 21-2-17.
//

#include "VulkanBase.h"

BEGIN_NAMESPACE(LearnVulkan)
VulkanBase::VulkanBase() {}

VulkanBase::~VulkanBase() {}

void VulkanBase::initVulkan() {
    createInstance();
    pickPhysicalDevice();
}

void VulkanBase::prepare() {}

void VulkanBase::prepareBase() {}

void VulkanBase::renderLoop() {}

void VulkanBase::renderFrame() {}

void VulkanBase::render() {}

void VulkanBase::defaultTouchOperation() {}

void VulkanBase::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "LearnVulkan";
    appInfo.pEngineName = "LearnVulkan";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};

    instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

    if (m_enabledInstanceExtensions.size() > 0) {
        for (auto enabledExtension : m_enabledInstanceExtensions) {
            instanceExtensions.push_back(enabledExtension);
        }
    }

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if (instanceExtensions.size() > 0) {
        if (m_debug) {
            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t) instanceExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    }
    if (m_debug) {
        // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
        // Note that on Android this layer requires at least NDK r20
        const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
        // Check if this layer is available at instance level
        uint32_t instanceLayerCount;
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
        bool validationLayerPresent = false;
        for (VkLayerProperties layer : instanceLayerProperties) {
            if (strcmp(layer.layerName, validationLayerName) == 0) {
                validationLayerPresent = true;
                break;
            }
        }
        if (validationLayerPresent) {
            instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
            instanceCreateInfo.enabledLayerCount = 1;
        } else {
            LOGI("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
        }
    }
    m_result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
}

void VulkanBase::pickPhysicalDevice() {
    uint32_t gpuCount = 0;
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr));
    assert(gpuCount > 0);
    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    m_result = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());

    // GPU selection

    // Select physical device to be used for the Vulkan example
    // Defaults to the first device unless specified by command line
    uint32_t selectedDevice = 0;

    m_physicalDevice = physicalDevices[selectedDevice];

    // Store properties (including limits), features and memory properties of the phyiscal device (so that examples can check against them)
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);

    // Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
    getDeviceFeatures();
}
END_NAMESPACE(LearnVulkan)