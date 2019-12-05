/*
* LearnVulkan QtVulkanWindow
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* Macro define of qtvulkanwindow
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
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
