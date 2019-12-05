/*
* LearnVulkan BasicEngine
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef VULKAN_BASICENGINE_HELP_H
#define VULKAN_BASICENGINE_HELP_H
// C++17 include
#include <iostream>
#include <memory>
#include <thread>
#include <ciso646>
#if defined(__GNUC__) && __GNUC__ > 7
    #include <filesystem>
    using fs=std::filesystem::path;
#else
    #include <experimental/filesystem>
    using fs=std::experimental::filesystem::path;
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//GLM

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Vulkan
#include <vulkan/vulkan.h>
#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1

#endif
