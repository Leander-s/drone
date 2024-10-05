cmake_minimum_required(VERSION 3.10)

set(PICO_SDK_PATH "../../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(drone_protocol C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(COMMUNICATIONS_HEADERS
    ../../debugging_util/include/debugging_config.h
    ../../debugging_util/include/debugging_util.h
    ../../diagnostics/include/diagnostics.h
    ../nrf24/include/nrf24_config.h
    ../nrf24/include/nrf24.h
    ../protocol_util/include/protocol_util.h
    include/protocol.h
    include/drone_protocol_config.h
)

set(COMMUNICATIONS_SOURCES
    ../../debugging_util/src/debugging_util.c
    ../../diagnostics/src/diagnostics.c
    ../nrf24/src/nrf24.c
    ../protocol_util/src/protocol_util.c
    src/protocol.c
)

set(COMMUNICATIONS_TEST_HEADERS
    ${COMMUNICATIONS_HEADERS}
)

set(COMMUNICATIONS_TEST_SOURCES
    ${COMMUNICATIONS_SOURCES}
    src/main.c
)

add_executable("drone_communications_test" ${COMMUNICATIONS_TEST_SOURCES}) 

pico_enable_stdio_usb("drone_communications_test" 1)

pico_add_extra_outputs("drone_communications_test")
target_include_directories("drone_communications_test" PRIVATE "./include" "../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../nrf24/include" "../../debugging_util/include" "../protocol_util/include" "../../drone/include" "../../diagnostics/include" "../../state/include" "../../math/include")
target_link_libraries("drone_communications_test" pico_stdlib hardware_spi hardware_pio hardware_uart)

add_library("drone_communications" ${COMMUNICATIONS_SOURCES})
pico_enable_stdio_usb("drone_communications" 1)

target_include_directories("drone_communications" PRIVATE "./include" "../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../nrf24/include" "../../debugging_util/include" "../protocol_util/include" "../../drone/include" "../../diagnostics/include" "../../state/include" "../../math/include")
target_link_libraries("drone_communications" pico_stdlib hardware_spi hardware_pio hardware_uart)
