cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
project(learnVulkan)

include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/GCC.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/FindOpenMP.cmake)

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


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/base)
include_directories(${PROJECT_SOURCE_DIR}/include/basicengine)
include_directories(${PROJECT_SOURCE_DIR}/third_party/gli)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glm)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/stb_image/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

include(${PROJECT_SOURCE_DIR}/cmake/FindVulkan.cmake)

add_subdirectory(src)
add_subdirectory(examples)
