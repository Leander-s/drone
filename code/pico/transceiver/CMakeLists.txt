cmake_minimum_required(VERSION 3.10)

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

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include(pico_sdk_import.cmake)
project(transceiver C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_INCLUDES
    "./src"
    "../debugging_util"
    "../nrf24"
    "../../general/diagnostics/include"
    "../../general/encoding/include"
)

set(PICO_SOURCES
    "./src"
    "../debugging_util"
    "../../general/diagnostics/src"
    "../nrf24"
    "../../general/encoding/src"
)

foreach(DIR ${PICO_SOURCES})
    file(GLOB FILE
        "${DIR}/*.c"
    )
    list(APPEND PICO_SOURCES ${FILE})
    list(FILTER PICO_SOURCES EXCLUDE REGEX "main.c")
    list(FILTER PICO_SOURCES EXCLUDE REGEX "test.c")
endforeach()

list(APPEND PICO_SOURCES "./src/main.c")

pico_sdk_init()

add_executable(${PROJECT_NAME} ${PICO_SOURCES})

pico_enable_stdio_usb(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${PICO_INCLUDES} "../../pico/pico-sdk/src/rp2_common/hardware_spi/include")
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pio hardware_uart)
