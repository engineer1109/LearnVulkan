cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(LIB_NAME VulkanEngine)

add_definitions(-DVULKANENGINE_EXPORT)

FILE(GLOB_RECURSE SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/*
        ${CMAKE_CURRENT_SOURCE_DIR}/base/*
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*
        ${CMAKE_CURRENT_SOURCE_DIR}/core/*
        ${CMAKE_CURRENT_SOURCE_DIR}/engines/*
        ${CMAKE_CURRENT_SOURCE_DIR}/template/*
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*
        )

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

if(BUILD_AdvExamples MATCHES ON)
    FILE(GLOB_RECURSE SRC_FILES_ASSIMP ${CMAKE_CURRENT_SOURCE_DIR}/plugins/assimp/*)

    add_library(${LIB_NAME}_assimp_plugin SHARED ${SRC_FILES_ASSIMP})

    target_link_libraries(${LIB_NAME}_assimp_plugin ${LIB_NAME} assimp)

    INSTALL(TARGETS ${LIB_NAME}_assimp_plugin DESTINATION "${PROJECT_SOURCE_DIR}/bin")
endif()

if(BUILD_QtExamples MATCHES ON)

    set(CMAKE_INCLUDE_CURRENT_DIR True)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    FILE(GLOB_RECURSE SRC_FILES_QT ${CMAKE_CURRENT_SOURCE_DIR}/plugins/qt/*)

    add_library(${LIB_NAME}_qt_plugin SHARED ${SRC_FILES_QT})

    target_link_libraries(${LIB_NAME}_qt_plugin ${LIB_NAME} Qt${QT_VERSION_MAJOR}::Widgets)

    INSTALL(TARGETS ${LIB_NAME}_qt_plugin DESTINATION "${PROJECT_SOURCE_DIR}/bin")

endif()
