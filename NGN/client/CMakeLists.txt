cmake_minimum_required(VERSION 2.8)

set (LINK_LIBS srvcore client msg_tools ngn_interface czmq zmq readline termcap)

include(${PROJECT_SOURCE_DIR}/env.txt)
include(${PROJECT_SOURCE_DIR}/env_def.txt)

find_package(BISON)
find_package(FLEX)

set(PARDEL_LOG  ${CMAKE_CURRENT_SOURCE_DIR}/srvparsedel.log)
add_custom_command(OUTPUT ${PARDEL_LOG}
    COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
    COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/lookup.c
)

set(PARSER_Y  ${CMAKE_CURRENT_SOURCE_DIR}/parser.y)
add_custom_command(OUTPUT ${PARSER_Y}
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bnf_parser.py -i  ${CMAKE_CURRENT_SOURCE_DIR}/cli.bnf -p ${CMAKE_CURRENT_SOURCE_DIR}/parser.y    
)

set(LOOK_UP  ${CMAKE_CURRENT_SOURCE_DIR}/lookup.c)
add_custom_command(OUTPUT ${LOOK_UP}
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/bnf_parser.py -i  ${CMAKE_CURRENT_SOURCE_DIR}/cli.bnf -l ${CMAKE_CURRENT_SOURCE_DIR}/lookup.c
)

add_custom_target(GenPyFile ALL DEPENDS ${PARDEL_LOG} ${PARSER_Y} ${LOOK_UP})
#add_custom_target(GenPyFile ALL DEPENDS ${PARSER_Y} ${LOOK_UP})

BISON_TARGET(PARSER parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.c)
FLEX_TARGET(LEXER lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

set(LIB_SRC ${FLEX_LEXER_OUTPUTS} ${BISON_PARSER_OUTPUTS} ${LOOK_UP} ahelp.c banner.c readline.c regex.c cli_trans.c optiwaycli.c)

message(STATUS "lib dir " ${LIB_DIR})
message(STATUS "lib  " ${LINK_LIBS})

include_directories(${INC_DIR})
link_directories(${LIB_DIR})
link_libraries(${LINK_LIBS})
add_definitions("-g -Wall")
add_executable(cli ${LIB_SRC})
