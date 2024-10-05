cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ground_protocol_pc)

set(INCLUDE_DIR
    "./include"
    "../../../math/include"
    "../../protocol_util/include"
    "../../../diagnostics/include"
    "../../../state/include"
)

set(COMMUNICATIONS_SOURCES
    ./src/protocol.c
    ./src/connection.c
    ../../protocol_util/src/protocol_util.c
    ../../../diagnostics/src/diagnostics.c
)

set(TEST_INCLUDES
    "./include"
    "./test_include"
    "../../../math/include"
    "../../protocol_util/include"
    "../../../diagnostics/include"
    "../../../state/include"
)

set(COMMUNICATIONS_TEST_SOURCES
    ./src/util.c
    ./src/connection.c
    ./test_src/test_protocol.c
    ./test_src/input_poll.c
    ./src/main.c
    ../../protocol_util/src/protocol_util.c
    ../../../diagnostics/src/diagnostics.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}) 

add_executable("ground_protocol_pc_test" ${COMMUNICATIONS_TEST_SOURCES})
target_include_directories("ground_protocol_pc_test" PRIVATE ${TEST_INCLUDES}) 
if(UNIX)
target_link_libraries("ground_protocol_pc_test" X11)
endif(UNIX)
