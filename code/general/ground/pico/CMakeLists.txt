cmake_minimum_required(VERSION 3.10)

set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR "./include")
file(GLOB LIBRARY_FILES "${LIBRARY_DIR}/*.a")

file(GLOB SOURCES
    "./src/*.c"
)

file(GLOB HEADERS
    "./src/*.h"
)

include(pico_sdk_import.cmake)
project(pico_transceiver C CXX ASM)
pico_sdk_init()


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES})
target_include_directories(${PROJECT_NAME} ${INCLUDE_DIR})
