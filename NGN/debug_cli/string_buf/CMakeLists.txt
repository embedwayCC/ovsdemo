set (LINK_LIBS msg_tools ngn_interface czmq zmq)
set (SRC_LIST look_up.c string_buf.c get_special_param.c error_code.c)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

if(PF STREQUAL "x86")
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/x86)
	set(lib_path ${PROJECT_SOURCE_DIR}/lib/x86)
else()
	set(CMAKE_C_COMPILER mips64-octeon-linux-gnu-gcc)
	set(CMAKE_CXX_COMPILER mips64-octeon-linux-gnu-gcc)
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/mips)		 
	set(lib_path ${PROJECT_SOURCE_DIR}/lib/mips)
endif()


include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})
add_definitions("-g -Wall")

add_library(client ${SRC_LIST})


