cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LEARN_VULKAN_PROJECT 04_MultiImageSampler)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
FILE(GLOB HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${LEARN_VULKAN_PROJECT} ${SRC_FILES} ${HEAD_FILES})
TARGET_LINK_LIBRARIES(${LEARN_VULKAN_PROJECT} vulkan_basicengine vulkanbase ${Vulkan_LIBRARIES})

INSTALL(TARGETS ${LEARN_VULKAN_PROJECT} DESTINATION "${PROJECT_SOURCE_DIR}/bin")

if(Win32)    #For windows VS cdebug can find data path.
    SET(BUILD_DIR ${PROJECT_SOURCE_DIR}/$(Configuration))
    configure_file(${PROJECT_SOURCE_DIR}/cmake/windows/vstemplate.vcxproj.user.in
        ${CMAKE_CURRENT_BINARY_DIR}/${LEARN_VULKAN_PROJECT}.vcxproj.user
        @ONLY
    )
endif()
