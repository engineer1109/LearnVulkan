#ifndef VULKAN_BASICENGINE_DEF_H
#define VULKAN_BASICENGINE_DEF_H

#ifdef VK_BASICENGINE_EXPORT_API
    #ifdef WIN32
        #define VK_BASICENGINE_EXPORT __declspec(dllexport)
    #else
        #define VK_BASICENGINE_EXPORT
    #endif
#else
    #define VK_BASICENGINE_EXPORT
#endif

#endif
