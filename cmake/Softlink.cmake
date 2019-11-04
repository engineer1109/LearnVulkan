cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
function(Softlink ag1)

if(UNIX)
message("Softlink ${ag1}")
add_custom_command(TARGET ${LEARN_VULKAN_PROJECT}
    PRE_BUILD
    COMMAND ln -s ${PROJECT_SOURCE_DIR}/data ${ag1}/../data
)
elseif(WIN32)
#add_custom_command(TARGET ${LEARN_VULKAN_PROJECT}
#    PRE_BUILD
#    COMMAND junction ${PROJECT_SOURCE_DIR}/data ${PROJECT_BINARY_DIR}/../data
#)
endif()

endfunction()
