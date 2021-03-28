# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("learnvulkan")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)
add_definitions(-DSTBI_NO_DDS)
add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

include_directories(
        ${PROJECT_SOURCE_DIR}/algorithm
        ${PROJECT_SOURCE_DIR}/base
        ${PROJECT_SOURCE_DIR}/core
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/engines
        ${PROJECT_SOURCE_DIR}/template
        ${PROJECT_SOURCE_DIR}/template/object
        ${PROJECT_SOURCE_DIR}/template/object/mesh
        ${PROJECT_SOURCE_DIR}/template/object/camera
        ${PROJECT_SOURCE_DIR}/template/object/shader
        ${PROJECT_SOURCE_DIR}/template/object/texture
        ${PROJECT_SOURCE_DIR}/utils
        ${PROJECT_SOURCE_DIR}/../../../../../third_party/glm
        ${PROJECT_SOURCE_DIR}/third_party/stb_image/include
        ${PROJECT_SOURCE_DIR}/../../../../../third_party/stb_image/include/
)

include_directories(
        ${PROJECT_SOURCE_DIR}/examples/01_StaticTriangle
        ${PROJECT_SOURCE_DIR}/examples/02_StaticCube
        ${PROJECT_SOURCE_DIR}/examples/03_Texture2dCube
        ${PROJECT_SOURCE_DIR}/examples/04_Skybox
)

add_library( # Sets the name of the library.
        VulkanBase

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        algorithm/ImageAlgorithm.cxx
        base/VulkanBase.cxx
        base/VulkanBaseEngine.cxx
        core/VulkanDescriptorSet.cxx
        core/VulkanVertexDescriptions.cxx
        core/VulkanPipelines.cxx
        engines/ThirdPersonEngine.cxx
        template/object/VulkanShader.cxx
        template/object/VulkanBuffer.cxx
        template/object/mesh/MeshObject.cxx
        template/object/mesh/VulkanCube.cxx
        template/object/camera/UniformCamera.cxx
        template/object/camera/ThirdPersonCamera.cxx
        template/object/texture/VulkanTexture2D.cxx
        template/object/texture/VulkanTextureCubeMap.cxx
        utils/VulkanTools.cpp

        ${PROJECT_SOURCE_DIR}/../../../../../third_party/stb_image/src/stb_image_aug.c
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        VulkanBase
        vulkan
        # Links the target library to the log library
        # included in the NDK.
        android
        ${log-lib} )

add_library(
        VulkanEngine

        SHARED
        native-lib.cpp
        examples/01_StaticTriangle/StaticTriangle.cxx
        examples/01_StaticTriangle/myobjects/Triangle.cxx
        examples/01_StaticTriangle/myobjects/TriangleShader.cxx
        examples/01_StaticTriangle/myobjects/TriangleUniform.cxx
        examples/02_StaticCube/StaticCube.cxx
        examples/03_Texture2dCube/Texture2dCube.cxx
        examples/04_Skybox/SkyboxCube.cxx
)

target_link_libraries(
        VulkanEngine
        VulkanBase
)