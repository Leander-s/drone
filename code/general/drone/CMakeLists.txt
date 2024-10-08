cmake_minimum_required(VERSION 3.10)

set(LIBRARY_DIR "./lib")
set(INCLUDE_DIR 
    "./include" 
    "../../../pico/pico-sdk/src/rp2_common/pico_stdio/include" 
    "../../../pico/pico-sdk/src/rp2_common/pico_stdlib/include"
    "../../../pico/pico-sdk/src/rp2_common/hardware_i2c/include"
    "../../../pico/pico-sdk/src/rp2_common/hardware_spi/include"
    "../../../pico/pico-sdk/src/rp2_common/hardware_pwm/include"
    "../communications/drone/include"
    "../communications/nrf24/include"
    "../communications/protocol_util/include"
    "../math/include"
    "../state/include"
    "../sensors/include"
    "../controls/include"
    "../controls/pwm_pico/include"
    "../debugging_util/include"
    "../diagnostics/include"
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
    "../controls/pwm_pico/src"
    "../debugging_util/src"
    "../diagnostics/src"
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

set(PICO_SDK_PATH "../../../pico/pico-sdk")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)
project(drone C CXX ASM)
pico_sdk_init()


add_executable(${PROJECT_NAME} ${SOURCES})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_FILES} pico_stdlib hardware_spi hardware_i2c hardware_pio hardware_uart m)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
