cmake_minimum_required(VERSION 3.10)

include(pico_sdk_import.cmake)
include_directories("/usr/include/x86_64-linux-gnu" "/usr/local/include" "/usr/include" "../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../nrf24" "../../debugging_util")
project(ground_protocol C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(PICO_HEADERS
    ../../debugging_util/debugging_config.h
    ../../debugging_util/debugging_util.h
    ../nrf24/nrf24_config.h
    ../nrf24/nrf24.h
)

set(PICO_SOURCES
    ../../debugging_util/debugging_util.c
    ../nrf24/nrf24.c
    src/pico_transceiver.c
)

set(COMMUNICATIONS_HEADERS
    src/util.h
    src/protocol.h
    src/input_poll.h
    src/connection.h
    src/ground_communications_config.h
)

set(COMMUNICATIONS_SOURCES
    src/util.c
    src/protocol.c
    src/input_poll.c
    src/connection.c
)

set(COMMUNICATIONS_TEST_HEADERS
    ${COMMUNICATIONS_HEADERS}
)

set(COMMUNICATIONS_TEST_SOURCES
    ${COMMUNICATIONS_SOURCES}
    src/main.c
)

add_executable("ground_communications_pico" ${PICO_SOURCES} ${PICO_HEADERS})

pico_enable_stdio_usb("ground_communications_pico" 1)

pico_add_extra_outputs("ground_communications_pico")
target_link_libraries("ground_communications_pico" pico_stdlib hardware_spi hardware_pio hardware_uart)

add_library("ground_communications" ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
target_link_libraries("ground_communications" X11)

add_executable("ground_communications_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})
target_link_libraries("ground_communications_test" X11)
