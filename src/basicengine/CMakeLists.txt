cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LIB_NAME vulkan_basicengine)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)
FILE(GLOB HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*
                     ${PROJECT_SOURCE_DIR}/include/basicengine/*)

include_directories(${PROJECT_SOURCE_DIR}/include/basicengine)
add_library(${LIB_NAME} SHARED ${SRC_FILES} ${IMGUI})
target_link_libraries(${LIB_NAME} vulkanbase ${Vulkan_LIBRARIES})
if(UNIX)
    target_link_libraries(${LIB_NAME} xcb)
endif()
