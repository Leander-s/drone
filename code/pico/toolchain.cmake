set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_DIR "${CMAKE_SOURCE_DIR}/../tools/toolchain/bin")

find_program(ARM_GCC arm-none-eabi-gcc)
if(NOT ARM_GCC)
    set(ARM_GCC "${TOOLCHAIN_DIR}/arm-none-eabi-gcc")
    set(ENV{PICO_TOOLCHAIN_PATH} "${TOOLCHAIN_DIR}")
endif()

find_program(ARM_GPP arm-none-eabi-g++)
if(NOT ARM_GPP)
    set(ARM_GPP "${TOOLCHAIN_DIR}/arm-none-eabi-g++")
    set(ENV{PICO_TOOLCHAIN_PATH} "${TOOLCHAIN_DIR}")
endif()

find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
if(NOT ARM_OBJCOPY )
    set(ARM_OBJCOPY "${TOOLCHAIN_DIR}/arm-none-eabi-objcopy")
    set(ENV{PICO_TOOLCHAIN_PATH} "${TOOLCHAIN_DIR}")
endif()

find_program(ARM_SIZE arm-none-eabi-size)
if(NOT ARM_SIZE)
    set(ARM_SIZE "${TOOLCHAIN_DIR}/arm-none-eabi-size")
    set(ENV{PICO_TOOLCHAIN_PATH} "${TOOLCHAIN_DIR}")
endif()

set(CMAKE_C_COMPILER "${ARM_GCC}")
set(CMAKE_CXX_COMPILER "${ARM_GPP}")
set(CMAKE_ASM_COMPILER "${ARM_GCC}")
set(CMAKE_OBJCOPY "${ARM_OBJCOPY}")
set(CMAKE_SIZE "${ARM_SIZE}")

message("C compiler is ${CMAKE_C_COMPILER}")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
