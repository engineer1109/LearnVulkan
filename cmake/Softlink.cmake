cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
message("CMake Version is ${CMAKE_MAJOR_VERSION} ${CMAKE_MINOR_VERSION}")
message("Support for windows symlink need 3.14 above. Less than 3.13 will copy files.")
#if(CMAKE_MINOR_VERSION GREATER 12)
#    message("CMAKE VERSION >=3.13 support create_symlink")
#else()
#    message("Old cmake. For windows create_symlink is abandoned.")
#endif()

function(Softlink ag1)

if(UNIX)
message("Softlink ${ag1}")
#add_custom_command(TARGET ${LEARN_VULKAN_PROJECT}
#    PRE_BUILD
#    COMMAND rm ${ag1}/../data
#    COMMAND ln -s ${PROJECT_SOURCE_DIR}/data ${ag1}/../data
#)
elseif(WIN32)
    #if(CMAKE_MINOR_VERSION GREATER 12)
    #    add_custom_command(TARGET ${LEARN_VULKAN_PROJECT}
    #        PRE_BUILD
    #        COMMAND cmake -E create_symlink ${PROJECT_SOURCE_DIR}/data ${ag1}/../data
    #    )
    #else()
        add_custom_command(TARGET ${LEARN_VULKAN_PROJECT}
            PRE_BUILD
            COMMAND cmake -E copy_directory ${PROJECT_SOURCE_DIR}/data ${ag1}/../data
        )
    #endif()
endif()

endfunction()
