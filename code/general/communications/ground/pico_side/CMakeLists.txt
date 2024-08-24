cmake_minimum_required(VERSION 3.10)

set(PICO_SDK_PATH "../../../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(ground_protocol_pico C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_HEADERS
    ../../../debugging_util/debugging_config.h
    ../../../debugging_util/debugging_util.h
    ../../../debugging_util/diagnostics.h
    ../../nrf24/nrf24_config.h
    ../../nrf24/nrf24.h
)

set(PICO_SOURCES
    ../../../debugging_util/debugging_util.c
    ../../../debugging_util/diagnostics.c
    ../../nrf24/nrf24.c
    src/pico_transceiver.c
)

pico_sdk_init()

add_executable(${PROJECT_NAME} ${PICO_SOURCES} ${PICO_HEADERS})

pico_enable_stdio_usb(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "../../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../../nrf24" "../../../debugging_util")
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pio hardware_uart)
