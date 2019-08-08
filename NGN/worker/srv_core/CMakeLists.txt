cmake_minimum_required(VERSION 2.8)

if(PF STREQUAL "x86")
set(LINK_LIBS cliport uci dl)
else()
set(LINK_LIBS cliport uci ukuser)
endif()

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LIB_SRC)
include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS}) 
add_library(srvcore ${LIB_SRC})
