cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LIB_NAME vulkan_basicengine)

add_definitions(-DSTBI_NO_DDS)
add_definitions(-DVK_BASICENGINE_EXPORT_API)
add_definitions(-DSTB_IMAGE_EXPORT_API)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)
FILE(GLOB HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*
                     ${PROJECT_SOURCE_DIR}/include/basicengine/*)
FILE(GLOB STB_IMAGE ${PROJECT_SOURCE_DIR}/third_party/stb_image/src/*
                     ${PROJECT_SOURCE_DIR}/third_party/stb_image/include/*)

include_directories(${PROJECT_SOURCE_DIR}/include/basicengine)
add_library(${LIB_NAME} SHARED ${SRC_FILES} ${HEAD_FILES} ${STB_IMAGE})
target_link_libraries(${LIB_NAME} vulkanbase ${Vulkan_LIBRARIES})
if(UNIX)
    target_link_libraries(${LIB_NAME} xcb stdc++fs)
endif()

INSTALL(TARGETS ${LIB_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/lib")
