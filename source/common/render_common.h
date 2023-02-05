//
// Created by wjl on 20-9-6.
//

#ifndef LEARNVULKAN_RENDER_COMMON_H
#define LEARNVULKAN_RENDER_COMMON_H

//Base
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <array>
#include <numeric>
#include <string>
#include <memory>
#include <algorithm>
#include <thread>
#include <functional>
#include <ciso646>
#include <random>

#ifdef VK_USE_PLATFORM_XCB_KHR
#include <xcb/xcb.h>
#endif

//GLM
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//Unix
#ifdef __unix__
#include <unistd.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingAPI.h>
#endif

//GL
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.h>

//Define

#ifndef BEGIN_NAMESPACE
#define BEGIN_NAMESPACE(n) namespace n {
#endif

#ifndef END_NAMESPACE
#define END_NAMESPACE(n) }
#endif

#ifndef interface
#define interface struct
#endif

//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vulkan", __VA_ARGS__))
//#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "vulkan", __VA_ARGS__))
//#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "vulkan", __VA_ARGS__))
//#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "vulkan", __VA_ARGS__))

#ifndef LOG
#define LOG(...) printf(__VA_ARGS__)
#endif

#ifndef LOGI
#define LOGI(...) printf(__VA_ARGS__)
#endif

#ifndef DELETE_PTR
#define DELETE_PTR(x) if(x) { delete x; x = nullptr; }
#endif

#ifndef VERTEX_BUFFER_BIND_ID
#define VERTEX_BUFFER_BIND_ID 0
#endif

#ifndef INSTANCE_BUFFER_BIND_ID
#define INSTANCE_BUFFER_BIND_ID 1
#endif

#endif //LEARNVULKAN_RENDER_COMMON_H
