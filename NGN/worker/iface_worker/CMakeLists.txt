set (LINK_LIBS srvapi libimatrix_rpc.so libpproto.so libprotobuf-c.so mpack srvcore pthread ngn_interface czmq zmq)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})
add_executable(if_worker ${SRC_LIST})


