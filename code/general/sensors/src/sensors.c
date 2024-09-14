#include <sensors.h>

void init_sensors(Sensor *sensor){
    imu_init(sensor->logs);
}

void update_sensors(Sensor *sensor){
    imu_read(sensor->state->orientation);
}
