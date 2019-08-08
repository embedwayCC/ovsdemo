message(STATUS "NGN/interface:This is CURRENT dir " ${CMAKE_CURRENT_SOURCE_DIR})

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)


include_directories(${INC_DIR})
set(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/interface)
set(LINK_LIBS libczmq.a libzmq.a)
set(SRC_LIST pipeline.c worker.c common.c client.c db.c )

link_directories(${LIB_DIR})

add_library(ngn_interface SHARED ${SRC_LIST})
