cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
project(drone_communications C)

set(HEADERS
    src/drone_protocol_config.h
    src/protocol.h
)

set(SOURCES
    src/protocol.c
)

set(TEST_HEADERS
    ${HEADERS}
)

set(TEST_SOURCES
    ${SOURCES}
    src/main.c
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable("drone_communications_test" ${TEST_SOURCES} ${TEST_HEADERS}) 
