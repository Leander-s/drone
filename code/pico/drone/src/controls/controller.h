#include "controls_math.h"
#include <stdint.h>
#include <stdlib.h>
#include <drone_states.h>

#define THROTTLE_MOTOR_N 1
#define PITCH_SERVO_LEFT_N 1
#define PITCH_SERVO_RIGHT_N 1
#define YAW_SERVO_N 1

typedef enum {
    Manual,
    Auto
} ControlMode;

typedef struct {
    Quaternion *current;

    int leftPitchServoIDs[PITCH_SERVO_LEFT_N];
    int rightPitchServoIDs[PITCH_SERVO_RIGHT_N];
    int yawServoIDs[YAW_SERVO_N];

    int throttleMotorIDs[THROTTLE_MOTOR_N];

    void (*servo_init)(int id);
    void (*servo_turn)(int id, float turn);
    void (*motor_init)(int id);
    void (*set_throttle)(int gp, float throttle);
} ControllerCreateInfo;

typedef struct {
    // target/current Quaternion are for autopilot
    Quaternion *target;
    Quaternion *current;

    Quaternion targetRate;
    Quaternion currentRate;

    DroneControlState controlState;

    // Not sure how many servos for what
    // Not sure how the wings will look
    int leftPitchServoIDs[PITCH_SERVO_LEFT_N] ;
    int rightPitchServoIDs[PITCH_SERVO_RIGHT_N] ;
    int yawServoIDs[YAW_SERVO_N];

    int throttleMotorIDs[THROTTLE_MOTOR_N];

    // function pointer to the function to turn a servo by an amount/with
    // a turn speed
    void (*servo_init)(int id);
    void (*servo_turn)(int id, float turn);
    void (*motor_init)(int id);
    void (*set_throttle)(int gp, float throttle);

    ControlMode mode;
} Controller;

Controller* controller_create(ControllerCreateInfo* createInfo);

// In case we use multithreading
void controller_run();

void controller_update(Controller* controller);

void controller_destroy(Controller* controller);
