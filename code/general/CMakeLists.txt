cmake_minimum_required(VERSION 3.10)

include(pico_sdk_import.cmake)
include_directories("../pico/pico-sdk/src/rp2_common/hardware_spi/include")
project(pico_drone C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(COMMUNICATIONS_HEADERS
    debugging_util/debugging_config.h
    debugging_util/debugging_util.h
    communications/nrf24/nrf24_config.h
    communications/nrf24/nrf24.h
    communications/drone_protocol/src/protocol.h
    communications/drone_protocol/src/drone_protocol_config.h
)

set(COMMUNICATIONS_SOURCES
    debugging_util/debugging_util.c
    communications/nrf24/nrf24.c
    communications/drone_protocol/src/protocol.c
)

set(COMMUNICATIONS_TEST_HEADERS
    ${COMMUNICATIONS_HEADERS}
)

set(COMMUNICATIONS_TEST_SOURCES
    ${COMMUNICATIONS_SOURCES}
    communications/drone_protocol/src/main.c
)

add_executable("drone_communications_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})

pico_enable_stdio_usb("drone_communications_test" 1)

pico_add_extra_outputs("drone_communications_test")
target_link_libraries("drone_communications_test" pico_stdlib hardware_spi hardware_pio hardware_uart)

add_library("drone_communications" ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
target_link_libraries("drone_communications" pico_stdlib hardware_spi hardware_pio hardware_uart)
