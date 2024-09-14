cmake_minimum_required(VERSION 3.10)

set(PICO_SDK_PATH "../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(drone_sensors C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
pico_sdk_init()

set(COMMUNICATIONS_HEADERS
    ../debugging_util/debugging_config.h
    ../debugging_util/debugging_util.h
    ../debugging_util/diagnostics.h
    ../math/general_math.h
    config.h
    sensors.h
    imu.h
)

set(COMMUNICATIONS_SOURCES
    ../debugging_util/debugging_util.c
    ../debugging_util/diagnostics.c
    ../math/general_math.c
    sensors.c
    imu.c
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

add_library("drone_sensors" ${COMMUNICATIONS_SOURCES} ${COMMUNICATIONS_HEADERS})
pico_enable_stdio_usb("drone_sensors" 1)

target_include_directories("drone_sensors" PRIVATE  "../math/" "../state/"  "../debugging_util/" "../../pico/pico-sdk/src/rp2_common/hardware_i2c/include/")
target_link_libraries("drone_sensors" pico_stdlib hardware_pio hardware_uart hardware_i2c m)
