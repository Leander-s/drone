cmake_minimum_required(VERSION 3.10)

project(ground_protocol_pc)

set(COMMUNICATIONS_HEADERS
    src/protocol.h
    src/connection.h
    src/ground_protocol_config.h
)

set(COMMUNICATIONS_SOURCES
    src/protocol.c
    src/connection.c
)

set(COMMUNICATIONS_TEST_HEADERS
    src/protocol.h
    src/test_config.h
    src/connection.h
    src/util.h
    src/input_poll.h
)

set(COMMUNICATIONS_TEST_SOURCES
    src/util.c
    src/connection.c
    src/test_protocol.c
    src/input_poll.c
    src/main.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})

add_executable("ground_protocol_pc_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
if(UNIX)
target_link_libraries("ground_protocol_pc_test" X11)
endif(UNIX)
