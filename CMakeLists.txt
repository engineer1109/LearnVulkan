cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(LearnVulkan)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/GCC.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindOpenMP.cmake)

add_definitions(-DSTBI_NO_DDS)
add_definitions(-DUSE_STD_FILESYSTEM)  # For Old Compilers as GCC 7 change to -DUSE_STD_EXPERIMENTAL_FILESYSTEM

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("Linux build needs libxcb-dev.")
    include(FindPkgConfig)
    pkg_check_modules(XCB xcb)
    if(XCB_FOUND)
        message("Found XCB ${XCB_LDFLAGS}")
    else()
        message("You may need install libxcb-dev!")
    endif()
endif()

#if(GIT_ThirdParty MATCHES ON)
#    message("Download third_party of glm gli imgui.")
#    message("git submodule update --init")
#    message("For Chinese, this will take long time.")
#    message("You can 'cmake .. -DGIT_ThirdParty=OFF' to turn off.(or cmake-gui ..)")
#    message("And manually put 'glm gli imgui' to 'third_party' dir.")
#    execute_process(COMMAND git submodule update --init
#                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#                OUTPUT_VARIABLE GIT_SUBMODULE)
#    message(GIT_SUBMODULE)
#endif()

if(WIN32)
    SET(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/)
    SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/)
else()
    SET(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
    SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
endif()


include_directories(${PROJECT_SOURCE_DIR}/third_party/gli)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glm)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/stb_image/include)
include_directories(${PROJECT_SOURCE_DIR}/source/algorithm)
include_directories(${PROJECT_SOURCE_DIR}/source/base)
include_directories(${PROJECT_SOURCE_DIR}/source/common)
include_directories(${PROJECT_SOURCE_DIR}/source/core)
include_directories(${PROJECT_SOURCE_DIR}/source/engines)
include_directories(${PROJECT_SOURCE_DIR}/source/plugins)
include_directories(${PROJECT_SOURCE_DIR}/source/plugins/qt)
include_directories(${PROJECT_SOURCE_DIR}/source/plugins/assimp)
include_directories(${PROJECT_SOURCE_DIR}/source/template)
include_directories(${PROJECT_SOURCE_DIR}/source/template/instance)
include_directories(${PROJECT_SOURCE_DIR}/source/template/object)
include_directories(${PROJECT_SOURCE_DIR}/source/template/object/camera)
include_directories(${PROJECT_SOURCE_DIR}/source/template/object/mesh)
include_directories(${PROJECT_SOURCE_DIR}/source/template/object/shader)
include_directories(${PROJECT_SOURCE_DIR}/source/template/object/texture)
include_directories(${PROJECT_SOURCE_DIR}/source/utils)
link_directories(${PROJECT_SOURCE_DIR}/lib)

include(${PROJECT_SOURCE_DIR}/cmake/FindVulkan.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindQt.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindAssimp.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Report.cmake)
#include(${PROJECT_SOURCE_DIR}/cmake/Softlink.cmake)

add_definitions(-DPROJECT_ABSOLUTE_PATH="${PROJECT_SOURCE_DIR}")

add_subdirectory(source)
add_subdirectory(examples)
