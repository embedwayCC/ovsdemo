#cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11") 
SET(INC_DIR ${PROJECT_SOURCE_DIR}/sde/include/)
#SET(LIB_DIR ${PROJECT_SOURCE_DIR}/sde/lib/)
#set(LINK_LIBS bf_switchd_lib bfsys bfutils driver stdc++) 
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(LIBTEST_SRC tna_exact_match.cpp)

include_directories(${INC_DIR})
#link_directories(${LIB_DIR})

ADD_LIBRARY(bf_rt_sdn STATIC ${LIBTEST_SRC})
#target_link_libraries(libtest ${LINK_LIBS})

