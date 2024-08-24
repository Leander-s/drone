cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ground_protocol_pc)

set(COMMUNICATIONS_HEADERS
    src/protocol.h
    src/connection.h
    src/ground_protocol_config.h
    ../../../debugging_util/diagnostics.h
)

set(COMMUNICATIONS_SOURCES
    src/protocol.c
    src/connection.c
    ../../../debugging_util/diagnostics.c
)

set(COMMUNICATIONS_TEST_HEADERS
    src/protocol.h
    src/test_config.h
    src/connection.h
    src/util.h
    src/input_poll.h
    ../../../debugging_util/diagnostics.h
)

set(COMMUNICATIONS_TEST_SOURCES
    src/util.c
    src/connection.c
    src/test_protocol.c
    src/input_poll.c
    src/main.c
    ../../../debugging_util/diagnostics.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "../../../debugging_util/") 

add_executable("ground_protocol_pc_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
target_include_directories("ground_protocol_pc_test" PRIVATE "../../../debugging_util/") 
if(UNIX)
target_link_libraries("ground_protocol_pc_test" X11)
endif(UNIX)
