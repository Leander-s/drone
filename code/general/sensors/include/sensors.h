#include "imu.h"

typedef struct{
    DroneSensorState *state;
    DroneSystemLog *logs;
}Sensor;

void init_sensors(Sensor *sensor);

void update_sensors(Sensor *sensor);
