#ifndef QTVULKANWINDOW_DEF_H
#define QTVULKANWINDOW_DEF_H

#ifdef VK_QTVULKANWINDOW_EXPORT_API
    #ifdef WIN32
        #define VK_QTVULKANWINDOW_EXPORT __declspec(dllexport)
    #else
        #define VK_QTVULKANWINDOW_EXPORT
    #endif
#else
    #define VK_QTVULKANWINDOW_EXPORT
#endif

#endif // QTVULKANWINDOW_DEF_H
