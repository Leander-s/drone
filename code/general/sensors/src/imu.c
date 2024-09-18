#include <hardware/i2c.h>
#include <imu.h>

void write_register(uint8_t reg, uint8_t data){
    uint8_t buf[2] = {reg, data};
    i2c_write_blocking(I2C_PORT, BNO055_ADDRESS, buf, 2, false);
}

uint8_t read_register(uint8_t reg){
    uint8_t data;
    i2c_write_blocking(I2C_PORT, BNO055_ADDRESS, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDRESS, &data, 1, false);
    return data;
}

void imu_init(DroneSystemLog *logs){
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // wait for sensor to start up, maybe wait a little less, see if that works
    sleep_ms(1000);

    uint8_t chip_id = read_register(BNO055_CHIP_ID_ADDR);
    if(chip_id != 0xA0){
        logs->error |= BNO055_INIT_ERROR;
        return;
    }

    write_register(BNO055_SYS_TRIGGER_ADDR, 0x20);
    sleep_ms(650);

    // normal power mode
    write_register(BNO055_PWR_MODE_ADDR, 0x00);

    // page 0 ??
    write_register(BNO055_PAGE_ID_ADDR, 0x00);

    write_register(BNO055_UNIT_SEL_ADDR, 0x04);

    // NDOF (Nine Degrees Of Freedom)
    // use all sensors
    write_register(BNO055_OPR_MODE_ADDR, 0x0C);

    sleep_ms(20);
}

void imu_read(DroneSensorState *state){
    uint8_t reg = BNO055_QUATERNION_DATA_W_LSB_ADDR;
    uint8_t data[8];

    i2c_write_blocking(I2C_PORT, BNO055_ADDRESS, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDRESS, data, 8, false);

    for(int i = 0; i < 8; i++){
        state->bytes[i] = data[i];
    }
    state->orientation.w = (float)((int16_t)((data[1] << 8) | data[0])) / 16384.0f;
    state->orientation.v.x = (float)((int16_t)((data[3] << 8) | data[2])) / 16384.0f;
    state->orientation.v.y = (float)((int16_t)((data[5] << 8) | data[4])) / 16384.0f;
    state->orientation.v.z = (float)((int16_t)((data[7] << 8) | data[6])) / 16384.0f;
}
