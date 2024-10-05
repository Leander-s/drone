cmake_minimum_required(VERSION 3.10)

set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR 
    "./include"
    "../../communications/ground/pico_side/include"
    "../../communications/nrf24/include"
    "../../communications/protocol_util/include"
    "../../debugging_util/include"
    "../../diagnostics/include"
    #[[
    "../../math/include"
    "../../state/include"
    ]]
)

set(SOURCE_DIR
    "./src"
    "../../communications/ground/pico_side/src"
    "../../communications/nrf24/src"
    "../../communications/protocol_util/src"
    "../../debugging_util/src"
    "../../diagnostics/src"
    #[[
    "../../math/src"
    "../../state/src"
    ]]
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

set(PICO_SDK_PATH "../../../../pico/pico-sdk")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)
project(pico_transceiver C CXX ASM)
pico_sdk_init()


add_executable(${PROJECT_NAME} ${SOURCES})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES} pico_stdlib hardware_spi hardware_pio hardware_uart)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
