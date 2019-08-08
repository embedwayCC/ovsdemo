cmake_minimum_required(VERSION 2.8)

set (link_libs mpack)
set (msg_tool_src js.c jsmn.c)
set (inc_dir ${PROJECT_SOURCE_DIR}/debug_cli/include
			 ${PROJECT_SOURCE_DIR}/msgpk_tools/include
			 ${PROJECT_SOURCE_DIR}/lib/mpack/include/)

if(PF STREQUAL "x86")
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/x86)
	set(lib_path ${PROJECT_SOURCE_DIR}/lib/x86)
else()
	set(CMAKE_C_COMPILER mips64-octeon-linux-gnu-gcc)
	set(CMAKE_CXX_COMPILER mips64-octeon-linux-gnu-gcc)
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mips)		 
	set(lib_path ${PROJECT_SOURCE_DIR}/lib/mips)
endif()

add_definitions("-g -Wall -std=c99 -D_GNU_SOURCE")
include_directories(${inc_dir})
link_directories(${lib_path})
link_libraries(${link_libs})
add_library(msg_tools ${msg_tool_src})

