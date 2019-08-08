set (LINK_LIBS mpack srvcore pthread ngn_interface czmq zmq msg_tools client bf_rt_sdn bf_switchd_lib bfsys bfutils driver stdc++)
set (SRC_LIST  om.c copy_to_cpu.c)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})

add_executable(om_worker ${SRC_LIST})


