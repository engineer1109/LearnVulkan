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

#endif //LEARNVULKAN_VULKAN_MACRO_H
