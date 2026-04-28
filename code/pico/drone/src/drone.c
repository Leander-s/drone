#include "drone.h"
#include "general_math.h"

void drone_start(Drone *drone) {
  drone->sensorState.altitude = 0;
  drone->log.error = 0;
  drone->sensor =
      (DroneSensor){.state = &drone->sensorState, .log = &drone->log};

  // create drone controls
  ControllerCreateInfo controllerInfo =
      (ControllerCreateInfo){.current = &drone->sensor.state->orientation,
                             .motor_init = init_throttle,
                             .set_throttle = set_throttle,
                             .servo_init = init_servo,
                             .servo_turn = set_angle,
                             .throttleMotorIDs[0] = 0,
                             .leftPitchServoIDs[0] = 5,
                             .rightPitchServoIDs[0] = 2,
                             .yawServoIDs[0] = 3};
  controller_init(&drone->controller, &controllerInfo);
  drone->state = drone->controller.controlState;

  // create drone transceiver
  DroneTransceiverCreateInfo createInfo =
      (DroneTransceiverCreateInfo){.init = nrf24_init,
                                   .send = nrf24_send,
                                   .recv = nrf24_read,
                                   .bufferSize = 32,
                                   .log = &drone->log,
                                   .controlState = &drone->state,
                                   .sensorState = drone->sensor.state};
  drone_protocol_init(&drone->transceiver, &createInfo);
  sensor_init(&drone->sensor);
}

void drone_update(Drone *drone) {
  // reading from transceiver
  drone_protocol_update(&drone->transceiver);

  // check if steering next
  // When steering, keep going
  // When not steering, stabelize drone by getting last rotation and correcting
  // it
  int steering = (drone->state.yaw != 127) || (drone->state.roll != 127) ||
                 (drone->state.pitch != 127);

  Quaternion prevRot = (Quaternion){
      .i = drone->sensor.state->orientation.i,
      .j = drone->sensor.state->orientation.j,
      .k = drone->sensor.state->orientation.k,
      .w = drone->sensor.state->orientation.w,
  };

  sensor_update(&drone->sensor);

  controller_update(&drone->controller);

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
  drone_protocol_terminate(&drone->transceiver);
  controller_destroy(&drone->controller);
}
