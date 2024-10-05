#pragma once
#include <sensors_config.h>

#define I2C_PORT i2c1
#define I2C_SDA 26
#define I2C_SCL 27

#define BNO055_ADDRESS 0x28
#define BNO055_CHIP_ID_ADDR 0x00
#define BNO055_ACCEL_DATA_X_LSB_ADDR 0x08
#define BNO055_EULER_H_LSB_ADDR 0x1A
#define BNO055_QUATERNION_DATA_W_LSB_ADDR 0x20
#define BNO055_OPR_MODE_ADDR 0x3D
#define BNO055_PWR_MODE_ADDR 0x3E
#define BNO055_SYS_TRIGGER_ADDR 0x3F
#define BNO055_PAGE_ID_ADDR 0x07
#define BNO055_UNIT_SEL_ADDR 0x3B

void imu_init(DroneSystemLog *logs);
void imu_read(DroneSensorState *state);
