cmake_minimum_required(VERSION 3.10)

set(PICO_SDK_PATH_REL "../../../pico/pico-sdk/")
set(TOOLCHAIN_DIR_REL "../../tools/toolchain/")

cmake_path(ABSOLUTE_PATH PICO_SDK_PATH_REL BASE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" NORMALIZE OUTPUT_VARIABLE PICO_SDK_PATH_ABS)
cmake_path(ABSOLUTE_PATH TOOLCHAIN_DIR_REL BASE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" NORMALIZE OUTPUT_VARIABLE TOOLCHAIN_DIR)

set(PICO_SDK_PATH "${PICO_SDK_PATH_ABS}")
set(TOOLCHAIN_DIR "${TOOLCHAIN_DIR}")

set(ENV{PICO_TOOLCHAIN_PATH} "${TOOLCHAIN_DIR}")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(pico_sdk_import.cmake)
project(drone_protocol C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(COMMUNICATIONS_INCLUDES
    ../../debugging_util/include/
    ../../diagnostics/include/
    ../nrf24/include/
    ../protocol_util/include/
    include/
)

set(COMMUNICATIONS_SOURCES
    src/protocol.c
)

set(COMMUNICATIONS_LIBRARIES
    ../nrf24/build/
    ../protocol_util/build/
    ../../debugging_util/build/
    ../../diagnostics/build/
)

set(COMMUNICATIONS_TEST_INCLUDES
    ${COMMUNICATIONS_INCLUDES}
)

set(COMMUNICATIONS_TEST_SOURCES
    ${COMMUNICATIONS_SOURCES}
    src/main.c
)

add_executable("drone_communications_test" ${COMMUNICATIONS_TEST_SOURCES}) 

pico_enable_stdio_usb("drone_communications_test" 1)

pico_add_extra_outputs("drone_communications_test")
target_include_directories("drone_communications_test" PRIVATE ${COMMUNICATIONS_TEST_INCLUDES} "../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../nrf24/include" "../../debugging_util/include" "../protocol_util/include" "../../drone/include" "../../diagnostics/include" "../../state/include" "../../math/include")
target_link_directories("drone_communications_test" PRIVATE "${COMMUNICATIONS_LIBRARIES}")
target_link_libraries("drone_communications_test" pico_stdlib hardware_spi hardware_pio hardware_uart nrf24 protocol_util debugging_util diagnostics)

add_library("drone_communications" ${COMMUNICATIONS_SOURCES})
pico_enable_stdio_usb("drone_communications" 1)

target_include_directories("drone_communications" PRIVATE ${COMMUNICATIONS_INCLUDES} "../pico/pico-sdk/src/rp2_common/hardware_spi/include" "../nrf24/include" "../../debugging_util/include" "../protocol_util/include" "../../drone/include" "../../diagnostics/include" "../../state/include" "../../math/include")
target_link_directories("drone_communications" PRIVATE "${COMMUNICATIONS_LIBRARIES}")
target_link_libraries("drone_communications" pico_stdlib hardware_spi hardware_pio hardware_uart nrf24 protocol_util debugging_util diagnostics)
