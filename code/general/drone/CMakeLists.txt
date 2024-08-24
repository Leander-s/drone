cmake_minimum_required(VERSION 3.10)

set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR "./include" "../../../pico/pico-sdk/src/rp2_common/pico_stdio/include" "../../../pico/pico-sdk/src/rp2_common/pico_stdlib/include")
file(GLOB LIBRARY_FILES "${LIBRARY_DIR}/*.a")

set(SOURCES
    src/main.c
)

set(HEADERS
    src/config.h
    include/drone_protocol_config.h
    include/protocol.h
)

set(PICO_SDK_PATH "../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(drone C CXX ASM)
pico_sdk_init()


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES} pico_stdlib hardware_spi hardware_pio hardware_uart)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
