#include <general_math.h>
#include <drone.h>

Drone *drone_start() {
  Drone *result = (Drone *)malloc(sizeof(Drone));
  result->sensorState.altitude = 0;
  result->log.error = 0;
  result->sensor =
      (DroneSensor){.state = &result->sensorState, .log = &result->log};

  // create drone controls
  ControllerCreateInfo controllerInfo =
      (ControllerCreateInfo){.current = &result->sensor.state->orientation,
                             .motor_init = init_throttle,
                             .set_throttle = set_throttle,
                             .servo_init = init_servo,
                             .servo_turn = set_angle,
                             .throttleMotorIDs[0] = 0,
                             .leftPitchServoIDs[0] = 1,
                             .rightPitchServoIDs[0] = 2,
                             .yawServoIDs[0] = 3};
  result->controller = controller_create(&controllerInfo);
  result->state = &result->controller->controlState;

  // create drone transceiver
  DroneTransceiverCreateInfo createInfo =
      (DroneTransceiverCreateInfo){.init = nrf24_init,
                                   .send = nrf24_send,
                                   .recv = nrf24_read,
                                   .bufferSize = 32,
                                   .log = &result->log,
                                   .controlState = result->state,
                                   .sensorState = result->sensor.state};
  result->transceiver = drone_protocol_init(&createInfo);
  sensor_init(&result->sensor);
  return result;
}

void drone_update(Drone *drone) {
  // reading from transceiver
  drone_protocol_update(drone->transceiver);

  // check if steering next
  // When steering, keep going
  // When not steering, stabelize drone by getting last rotation and correcting
  // it
  int steering = (drone->state->yaw != 127) || (drone->state->roll != 127) || 
                 (drone->state->pitch != 127);

  Quaternion prevRot = (Quaternion){
      .i = drone->sensor.state->orientation.i,
      .j = drone->sensor.state->orientation.j,
      .k = drone->sensor.state->orientation.k,
      .w = drone->sensor.state->orientation.w,
  };

  sensor_update(&drone->sensor);

  controller_update(drone->controller);

  /*
   * Automatically stabelizing, not implemented fully yet
  // update the controls
  if (steering) {
    controller_update(drone->controller);
    return;
  }

  // if not steering, compensate for errors
  Quaternion prevInv = (Quaternion){
      .w = prevRot.w,
      .i = -prevRot.i,
      .j = -prevRot.j,
      .k = -prevRot.k,
  };

  Quaternion relativeRot;

  mult_quat_quat(&drone->sensor.state->orientation, &prevInv, &relativeRot);

  Quaternion temp;
  Quaternion localRot;

  mult_quat_quat(&relativeRot, &drone->sensor.state->orientation, &temp);

  mult_quat_quat(&prevInv, &temp, &localRot);
  
  // still need to correct after this

  //  controller_update(drone->controller);
  */
}

void drone_end(Drone *drone) {
  drone_protocol_terminate(drone->transceiver);
  controller_destroy(drone->controller);
  free(drone);
}
