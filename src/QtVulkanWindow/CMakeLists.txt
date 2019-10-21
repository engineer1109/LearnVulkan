cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LIB_NAME qtvulkanwindow)

add_definitions(-DVK_QTVULKANWINDOW_EXPORT_API)

set(CMAKE_INCLUDE_CURRENT_DIR True)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)
FILE(GLOB HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*
                     ${PROJECT_SOURCE_DIR}/include/qtvulkanwindow/*)

include_directories(${PROJECT_SOURCE_DIR}/include/qtvulkanwindow)
add_library(${LIB_NAME} SHARED ${SRC_FILES} ${HEAD_FILES} ${STB_IMAGE})
target_link_libraries(${LIB_NAME} Qt5::Widgets Qt5::Core vulkan_basicengine vulkanbase ${Vulkan_LIBRARIES})
if(UNIX)
    target_link_libraries(${LIB_NAME} xcb)
endif()

INSTALL(TARGETS ${LIB_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/lib")
