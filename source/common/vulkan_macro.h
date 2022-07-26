//
// Created by wjl on 21-2-23.
//

#ifndef LEARNVULKAN_VULKAN_MACRO_H
#define LEARNVULKAN_VULKAN_MACRO_H

#include "render_common.h"

#define VK_SAFE_DELETE(var, f)																				\
{		                               \
    if(var  != VK_NULL_HANDLE){        \
        (f);                           \
        var = VK_NULL_HANDLE;          \
    }								   \
}

#ifdef VULKANENGINE_EXPORT
#ifdef WIN32
#define VULKANENGINE_EXPORT_API __declspec(dllexport)
#else
#define VULKANENGINE_EXPORT_API __attribute__((visibility("default")))
#endif
#else
#define VULKANENGINE_EXPORT_API
#endif

#endif //LEARNVULKAN_VULKAN_MACRO_H
