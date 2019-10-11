#ifndef VULKAN_BASICENGINE_HELP_H
#define VULKAN_BASICENGINE_HELP_H
#include <iostream>
#include <memory>
#include <thread>
#if defined(__GNUC__) && __GNUC__ > 7
    #include <filesystem>
#else
    #include <experimental/filesystem>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#endif
