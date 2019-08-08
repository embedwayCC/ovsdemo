message(STATUS "NGN/broker:This is CURRENT dir " ${CMAKE_CURRENT_SOURCE_DIR})

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)
 
include_directories(${INC_DIR})
set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/broker)
set(LINK_LIBS ngn_interface czmq zmq mpack pthread) 
set(SRC_LIST broker.c ctx.c db_local.c mdb.c midl.c)
 
link_directories(${LIB_DIR})
add_executable(broker ${SRC_LIST})
target_link_libraries(broker ${LINK_LIBS})
