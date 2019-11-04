SET(LEARN_VULKAN_PROJECT Qt_01_Triangle)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
FILE(GLOB HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
FILE(GLOB UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${LEARN_VULKAN_PROJECT} ${SRC_FILES} ${HEAD_FILES} ${UI_FILES})

TARGET_LINK_LIBRARIES(${LEARN_VULKAN_PROJECT} qtvulkanwindow)

set_property(TARGET ${LEARN_VULKAN_PROJECT} PROPERTY CXX_STANDARD 17)

if(Win32)    #For windows VS cdebug can find data path.
    SET(BUILD_DIR ${PROJECT_SOURCE_DIR}/$(Configuration))
    configure_file(${PROJECT_SOURCE_DIR}/cmake/windows/vstemplate.vcxproj.user.in
        ${CMAKE_CURRENT_BINARY_DIR}/${LEARN_VULKAN_PROJECT}.vcxproj.user
        @ONLY
    )
endif()
