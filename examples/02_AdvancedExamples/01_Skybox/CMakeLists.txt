cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Adv_01_Skybox)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#message("opencv include dir")
#message(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIBS})


#set(LIBS ${LIBS} ${OpenCV_LIBS})


#add_executable(main main.cxx)
#target_link_libraries(main ${LIBS} pthread)
#set_property(TARGET main PROPERTY CXX_STANDARD 11)
