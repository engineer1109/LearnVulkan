cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LIB_NAME VulkanEngine)

add_definitions(-DVULKANENGINE_EXPORT)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)

FILE(GLOB_RECURSE IMGUI ${CMAKE_SOURCE_DIR}/third_party/imgui/*)

FILE(GLOB_RECURSE STB_IMAGE_SOURCE ${CMAKE_SOURCE_DIR}/third_party/stb_image/src/*
        ${CMAKE_SOURCE_DIR}/third_party/stb_image/include/*)

add_library(${LIB_NAME} SHARED ${SRC_FILES} ${IMGUI} ${STB_IMAGE_SOURCE})

if(UNIX)
    target_link_libraries(${LIB_NAME} ${Vulkan_LIBRARIES} xcb)
else()
    target_link_libraries(${LIB_NAME} ${Vulkan_LIBRARIES})
endif()

INSTALL(TARGETS ${LIB_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/bin")