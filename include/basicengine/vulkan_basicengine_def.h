/*
* LearnVulkan BasicEngine
*
* Copyright (C) by Jialiang Wang - https://github.com/engineer1109/LearnVulkan
* 1292846099@qq.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
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
