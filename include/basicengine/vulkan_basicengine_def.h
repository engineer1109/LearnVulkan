/*
* LearnVulkan BasicEngine
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
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

#define BEGIN_NAMESPACE(n) namespace n{

#define END_NAMESPACE(n) }

#endif
