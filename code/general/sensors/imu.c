#include "imu.h"

void imu_init(DroneSystemLog *logs){
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // wait for sensor to start up, maybe wait a little less, see if that works
    sleep_ms(1000);

    uint8_t chip_id = 0;
    uint8_t reg = BNO055_CHIP_ID_ADDR;
    i2c_write_blocking(I2C_PORT, BNO055_ADDRESS, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDRESS, &chip_id, 1, false);

    if(chip_id != 0xA0){
        logs->error |= BNO055_INIT_ERROR;
        return;
    }

    // Set operation mode ??
}

void imu_read(Quaternion *quat){
    uint8_t reg = BNO055_QUATERNION_DATA_W_LSB_ADDR;
    uint8_t data[8];

    i2c_write_blocking(I2C_PORT, BNO055_ADDRESS, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDRESS, data, 8, false);

    quat->w = (float)((int16_t)((data[1] << 8) | data[0])) / 16384.0f;
    quat->v.x = (float)((int16_t)((data[3] << 8) | data[2])) / 16384.0f;
    quat->v.y = (float)((int16_t)((data[5] << 8) | data[4])) / 16384.0f;
    quat->v.z = (float)((int16_t)((data[7] << 8) | data[6])) / 16384.0f;
}
