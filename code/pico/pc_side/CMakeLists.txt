cmake_minimum_required(VERSION 3.13)

project(test_pc C)

set(SOURCES
    src/main.c
    src/util.c
    src/input_poll.c
    src/connection.c
)

set(HEADERS
    src/config.h
    src/util.h
    src/input_poll.h
    src/connection.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} X11)
