if(BUILD_QtExamples MATCHES ON)
    set(CMAKE_INCLUDE_CURRENT_DIR True)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    include_directories(${PROJECT_SOURCE_DIR}/source/plugins/qt)
    add_subdirectory(21_QtVulkanSceneDemo)
endif()
