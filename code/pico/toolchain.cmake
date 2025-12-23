set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_DIR "${CMAKE_SOURCE_DIR}/../tools/toolchain/bin")
set(CMAKE_C_COMPILER "${TOOLCHAIN_DIR}/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_DIR}/arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_DIR}/arm-none-eabi-gcc")

set(CMAKE_OBJCOPY "${TOOLCHAIN_DIR}/arm-none-eabi-objcopy")
set(CMAKE_SIZE "${TOOLCHAIN_DIR}/arm-none-eabi-size")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
