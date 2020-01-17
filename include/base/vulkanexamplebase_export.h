#ifndef VULKANEXAMPLEBASE_EXPORT_H
#define VULKANEXAMPLEBASE_EXPORT_H

#ifdef VK_BASE_EXPORT_API
    #ifdef WIN32
        #define VK_BASE_EXPORT __declspec(dllexport)
    #else
        #define VK_BASE_EXPORT
    #endif
#else
    #define VK_BASE_EXPORT
#endif

#endif
