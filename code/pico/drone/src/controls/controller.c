#include "controller.h"

#include <stdlib.h>

void controller_init(Controller *controller, ControllerCreateInfo *createInfo) {
  controller->mode = Manual;

  // init function
  controller->servo_init = createInfo->servo_init;
  controller->servo_turn = createInfo->servo_turn;
  controller->motor_init = createInfo->motor_init;
  controller->set_throttle = createInfo->set_throttle;

  // init motor id and run function
  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->throttleMotorIDs[i] = createInfo->throttleMotorIDs[i];
    controller->motor_init(controller->throttleMotorIDs[i]);
  }

  // init servo ids
  for (int i = 0; i < PITCH_SERVO_LEFT_N; i++) {
    controller->leftPitchServoIDs[i] = createInfo->leftPitchServoIDs[i];
    controller->servo_init(controller->leftPitchServoIDs[i]);
  }
  for (int i = 0; i < PITCH_SERVO_RIGHT_N; i++) {
    controller->rightPitchServoIDs[i] = createInfo->rightPitchServoIDs[i];
    controller->servo_init(controller->rightPitchServoIDs[i]);
  }
  for (int i = 0; i < YAW_SERVO_N; i++) {
    controller->yawServoIDs[i] = createInfo->yawServoIDs[i];
    controller->servo_init(controller->yawServoIDs[i]);
  }

  // init control state
  controller->controlState =
      (DroneControlState){.yaw = 127, .pitch = 127, .roll = 127, .throttle = 0};
}

void controller_update(Controller *controller) {

  // map controls to angle
  float yawAngle = (float)(controller->controlState.yaw) / 255.0f * 180.0f;
  float pitchAngle = (float)(controller->controlState.pitch) / 255.0f * 180.0f;
  float rollAngle =
      ((float)(controller->controlState.roll) - 127.0f) / 255.0f * 180.0f;

  // need to do some math to interpolate between pitch and roll
  for (int i = 0; i < PITCH_SERVO_LEFT_N; i++) {
    // turn pitch servos the way they have to turn
    float angle = clamp_float(pitchAngle - rollAngle, 0.0f, 180.0f);
    controller->servo_turn(controller->leftPitchServoIDs[i], angle);
  }
  for (int i = 0; i < PITCH_SERVO_RIGHT_N; i++) {
    // turn pitch servos the way they have to turn
    float angle = clamp_float(pitchAngle + rollAngle, 0.0f, 180.0f);
    controller->servo_turn(controller->rightPitchServoIDs[i], angle);
  }
  for (int i = 0; i < YAW_SERVO_N; i++) {
    // turn yaw servos the way they have to turn
    controller->servo_turn(controller->yawServoIDs[i], yawAngle);
  }

  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->set_throttle(controller->throttleMotorIDs[i],
                             (float)(controller->controlState.throttle) / 255);
  }
}

void controller_destroy(Controller *controller) {
  for (int i = 0; i < THROTTLE_MOTOR_N; i++) {
    controller->set_throttle(controller->throttleMotorIDs[i], 0.0f);
  }
};
