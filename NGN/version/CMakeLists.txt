cmake_minimum_required(VERSION 2.8)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

if(PF STREQUAL "x86")
    set(BIN "x86")
else()
    set(BIN "mips")
endif()

set(VER_LOG  ${CMAKE_CURRENT_SOURCE_DIR}/pack_version.log)
set(SCRIPT_DIR ${PROJECT_SOURCE_DIR}/version/scripts)

add_custom_command(OUTPUT ${VER_LOG}
    COMMAND sh ${SCRIPT_DIR}/pack.sh -n ngn -p ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/release/
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/release/*
    COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/ngn_*.tar.gz ${PROJECT_SOURCE_DIR}/release/
)

add_custom_target(PackVersion ALL DEPENDS ${VER_LOG})
