#include <sensors.h>

void sensor_init(DroneSensor *sensor){
    imu_init(sensor->log);
}

void sensor_update(DroneSensor *sensor){
    imu_read(sensor->state);
}
