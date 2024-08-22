cmake_minimum_required(VERSION 3.10)

project(ground_protocol_pc)

set(HEADERS
    src/util.h
    src/input_poll.h
    src/config.h
)

set(SOURCE
    src/util.c
    src/input_poll.c
)

add_library(${PROJECT_NAME} ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
if(UNIX)
target_link_libraries(${PROJECT_NAME} X11)
endif(UNIX)

add_executable("pc_gui_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
if(UNIX)
target_link_libraries("pc_gui_test" X11)
endif(UNIX)
