set (LINK_LIBS client msg_tools ngn_interface czmq zmq)
set (SRC_LIST optiwaycli.c)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})
add_definitions("-g -Wall")

add_executable(debug_cli ${SRC_LIST})


