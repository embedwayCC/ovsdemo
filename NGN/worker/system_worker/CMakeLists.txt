set (LINK_LIBS mpack srvcore pthread ngn_interface czmq zmq msg_tools client)
set (SRC_LIST  srv_sys.c)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})

add_executable(sys_worker ${SRC_LIST})


