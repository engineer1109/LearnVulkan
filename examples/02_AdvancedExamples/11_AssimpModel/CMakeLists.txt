cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LEARN_VULKAN_PROJECT 11_AssimpModel)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/* ${CMAKE_CURRENT_SOURCE_DIR}/myobjects/*)

add_executable(${LEARN_VULKAN_PROJECT} ${SRC_FILES})
TARGET_LINK_LIBRARIES(${LEARN_VULKAN_PROJECT} VulkanEngine_assimp_plugin VulkanEngine ${Vulkan_LIBRARIES})

set_work_dir_function(${LEARN_VULKAN_PROJECT})

INSTALL(TARGETS ${LEARN_VULKAN_PROJECT} DESTINATION "${PROJECT_SOURCE_DIR}/bin")