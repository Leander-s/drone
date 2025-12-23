cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(protocol_util LANGUAGES C)

set(INCLUDE_DIR
    "./include"
)

set(PROTOCOL_UTIL_SOURCES
    ./src/protocol_util.c
)

set(TEST_INCLUDES
    "./include"
)

set(PROTOCOL_UTIL_TEST_SOURCES
    ./src/protocol_util.c
)

add_library(${PROJECT_NAME} ${PROTOCOL_UTIL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}) 
