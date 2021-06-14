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

//Unix
#include <unistd.h>

//GL
#include <vulkan/vulkan.h>

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

//Android
#if defined(__ANDROID__)
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>
#endif

//Define

#define BEGIN_NAMESPACE(n) namespace n {

#define END_NAMESPACE(n) }

#define interface class

//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vulkan", __VA_ARGS__))
//#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "vulkan", __VA_ARGS__))
//#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "vulkan", __VA_ARGS__))
//#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "vulkan", __VA_ARGS__))

#define DELETE_PTR(x) if(x) { delete x; x = nullptr; }

#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1

#endif //LEARNVULKAN_RENDER_COMMON_H
