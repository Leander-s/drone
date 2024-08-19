cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include_directories("../pico-sdk/src/rp2_common/hardware_spi/include" "../nrf24/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
project(pico_receiver C CXX ASM)
pico_sdk_init()

set(SOURCES
    src/main.c
    ../nrf24/nrf24.c
    ../nrf24/util.c
)

set(HEADERS
    src/config.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pio hardware_uart)
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
