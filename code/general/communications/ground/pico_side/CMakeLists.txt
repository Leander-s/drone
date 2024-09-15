cmake_minimum_required(VERSION 3.10)

set(PICO_SDK_PATH "../../../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(ground_protocol_pico C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_HEADERS
    ./include
    ../../../debugging_util/include/
    ../../../diagnostics/include/
    ../../nrf24/include/
    ../../protocol_util/include/
)

set(PICO_SOURCES
    ../../../debugging_util/src/debugging_util.c
    ../../../diagnostics/src/diagnostics.c
    ../../nrf24/src/nrf24.c
    ../../protocol_util/src/protocol_util.c
    src/pico_transceiver.c
    test/main.c
)

pico_sdk_init()

add_executable(${PROJECT_NAME} ${PICO_SOURCES})

pico_enable_stdio_usb(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${PICO_HEADERS} "../../pico/pico-sdk/src/rp2_common/hardware_spi/include")
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pio hardware_uart)
