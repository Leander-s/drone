#include <sensors.h>

void sensor_init(Sensor *sensor){
    imu_init(sensor->log);
}

void sensor_update(Sensor *sensor){
    imu_read(&sensor->state->orientation);
}
