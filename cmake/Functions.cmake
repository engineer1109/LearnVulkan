function(set_work_dir_function pro_name)
if(WIN32)
    SET_TARGET_PROPERTIES(${pro_name}
                          PROPERTIES
                          VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
                          )
endif()
endfunction()