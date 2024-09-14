#include <hardware/structs/io_bank0.h>
#include <general_math.h>
#include <drone_states.h>
#include <diagnostics.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define I2C_PORT i2c1
#define I2C_SDA 26
#define I2C_SCL 27

#define BNO055_ADDRESS 0x28
#define BNO055_CHIP_ID_ADDR 0x00
#define BNO055_ACCEL_DATA_X_LSB_ADDR 0x08
#define BNO055_EULER_H_LSB_ADDR 0x1A
#define BNO055_QUATERNION_DATA_W_LSB_ADDR 0x20
