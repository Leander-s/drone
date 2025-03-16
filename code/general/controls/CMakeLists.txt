cmake_minimum_required(VERSION 3.10)

set(PICO_SDK_PATH "../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(drone_controls C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(INCLUDE_DIRS
    ../debugging_util/include
    ../diagnostics/include
    ../math/include
    ../state/include
    include/
)

set(COMMUNICATIONS_SOURCES
    ../debugging_util/src/debugging_util.c
    ../diagnostics/src/diagnostics.c
    ../math/src/general_math.c
    src/controller.c
)

#[[
set(COMMUNICATIONS_TEST_HEADERS
    ${COMMUNICATIONS_HEADERS}
)

set(COMMUNICATIONS_TEST_SOURCES
    ${COMMUNICATIONS_SOURCES}
    src/main.c
)
]]

#[[
add_executable("drone_sensors_test" ${COMMUNICATIONS_TEST_SOURCES} ${COMMUNICATIONS_TEST_HEADERS})

pico_enable_stdio_usb("drone_sensors_test" 1)

pico_add_extra_outputs("drone_sensors_test")
target_include_directories("drone_sensors_test" PRIVATE "../debugging_util" "../state/")
target_link_libraries("drone_sensors_test" pico_stdlib hardware_pio hardware_uart hardware_i2c m)
]]

add_library("drone_controls" ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
pico_enable_stdio_usb("drone_controls" 1)

target_include_directories("drone_controls" PRIVATE ${INCLUDE_DIRS} "../../pico/pico-sdk/src/rp2_common/hardware_i2c/include/")
target_link_libraries("drone_controls" pico_stdlib hardware_pio hardware_uart hardware_i2c m)
