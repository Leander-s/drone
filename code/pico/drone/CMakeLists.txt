cmake_minimum_required(VERSION 3.13)

set(INCLUDE_DIR 
    "./src" 
    "./src/communications"
    "./src/sensors"
    "./src/controls"
    "../debugging_util/"
    "../nrf24/"
    "../../pico/pico-sdk/src/rp2_common/pico_stdio/include" 
    "../../pico/pico-sdk/src/rp2_common/pico_stdlib/include"
    "../../pico/pico-sdk/src/rp2_common/hardware_i2c/include"
    "../../pico/pico-sdk/src/rp2_common/hardware_spi/include"
    "../../pico/pico-sdk/src/rp2_common/hardware_pwm/include"
    "../../general/encoding/include"
    "../../general/math/include"
    "../../general/state/include"
    "../../general/diagnostics/include"
)

set(SOURCE_DIR
    "./src"
    "./src/sensors"
    "./src/controls"
    "./src/communications"
    "../debugging_util"
    "../nrf24"
    "../../general/encoding/src"
    "../../general/math/src"
    "../../general/state/src"
    "../../general/diagnostics/src"
)

foreach(DIR ${SOURCE_DIR})
    file(GLOB FILE
        "${DIR}/*.c"
    )
    list(APPEND SOURCES ${FILE})
    list(FILTER SOURCES EXCLUDE REGEX "main.c")
    list(FILTER SOURCES EXCLUDE REGEX "test.c")
endforeach()

list(APPEND SOURCES src/main.c)

set(PICO_SDK_PATH_REL "../../pico/pico-sdk/")
set(TOOLCHAIN_DIR_REL "../tools/toolchain/")
set(PICO_TOOL_DIR_REL "../tools/picotool/")

cmake_path(ABSOLUTE_PATH PICO_SDK_PATH_REL BASE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" NORMALIZE OUTPUT_VARIABLE PICO_SDK_PATH_ABS)
cmake_path(ABSOLUTE_PATH TOOLCHAIN_DIR_REL BASE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" NORMALIZE OUTPUT_VARIABLE TOOLCHAIN_DIR)
cmake_path(ABSOLUTE_PATH PICO_TOOL_DIR_REL BASE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" NORMALIZE OUTPUT_VARIABLE PICO_TOOL_DIR)

set(PICO_SDK_PATH "${PICO_SDK_PATH_ABS}")
set(TOOLCHAIN_DIR "${TOOLCHAIN_DIR}")
set(picotool_DIR "${PICO_TOOL_DIR}")

set(ENV{PICO_TOOLCHAIN_PATH} "${TOOLCHAIN_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)
project(drone C CXX ASM)
pico_sdk_init()


add_executable(${PROJECT_NAME} ${SOURCES})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES} pico_stdlib hardware_spi hardware_i2c hardware_pio hardware_uart m)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
