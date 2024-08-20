cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(ground_protocol_pico C CXX ASM)

set(PICO_HEADERS
    ../../../debugging_util/debugging_config.h
    ../../../debugging_util/debugging_util.h
    ../../nrf24/nrf24_config.h
    ../../nrf24/nrf24.h
)

set(PICO_SOURCES
    ../../../debugging_util/debugging_util.c
    ../../nrf24/nrf24.c
    src/pico_transceiver.c
)

pico_sdk_init()

add_executable("${PROJECT_NAME}_test" ${PICO_SOURCES} ${PICO_HEADERS})
add_library(${PROJECT_NAME} ${PICO_SOURCES} ${PICO_HEADERS})

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_usb("${PROJECT_NAME}_test" 1)

pico_add_extra_outputs("${PROJECT_NAME}_test")

target_include_directories(${PROJECT_NAME} PRIVATE "../../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../../nrf24" "../../../debugging_util")
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pio hardware_uart)
target_include_directories("${PROJECT_NAME}_test" PRIVATE "../../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../../nrf24" "../../../debugging_util")
target_link_libraries("${PROJECT_NAME}_test" pico_stdlib hardware_spi hardware_pio hardware_uart)

