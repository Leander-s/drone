#pragma once
#include <sensors_config.h>

void imu_init(DroneSystemLog *logs);
void imu_read(Quaternion *quat);
