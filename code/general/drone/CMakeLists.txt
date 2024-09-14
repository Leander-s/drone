cmake_minimum_required(VERSION 3.10)

set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR 
    "./include" 
    "../../../pico/pico-sdk/src/rp2_common/pico_stdio/include" 
    "../../../pico/pico-sdk/src/rp2_common/pico_stdlib/include"
    "../communications/drone/include"
    "../communications/nrf24/include"
    "../communications/protocol_util/include"
    "../math/include"
    "../state/include"
    "../sensors/include"
    "../controls/include"
    "../debugging_util/include"
)
set(SOURCE_DIR
    "./src"
    "../communications/drone/src"
    "../communications/nrf24/src"
    "../communications/protocol_util/src"
    "../math/src"
    "../state/src"
    "../sensors/src"
    "../controls/src"
    "../debugging_util/src"
)

file(GLOB SOURCES "${SOURCE_DIR}/*.c" EXCEPT main.c test.c)

set(PICO_SDK_PATH "../../../pico/pico-sdk")

include(pico_sdk_import.cmake)
project(drone C CXX ASM)
pico_sdk_init()


add_executable(${PROJECT_NAME} src/main.c ${SOURCES})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES} pico_stdlib hardware_spi hardware_pio hardware_uart)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
