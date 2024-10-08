#include "controller_config.h"
#include "controls_math.h"

#define PITCH_SERVO_N 1
#define YAW_SERVO_N 1
#define ROLL_SERVO_N 1

typedef enum {
    Manual,
    Auto
} ControlMode;

typedef struct {
    // target/current Quaternion are for autopilot
    Quaternion *target;
    Quaternion *current;

    Quaternion targetRate;
    Quaternion currentRate;

    DroneControlState controlState;

    // Not sure how many servos for what
    // Not sure how the wings will look
    int pitchServoIDs[1];
    int yawServoIDs[1];
    int rollServoIDs[1];

    int throttleMotorID;

    // function pointer to the function to turn a servo by an amount/with
    // a turn speed
    void (*servo_turn)(int id, int turn);
    void (*set_throttle)(int gp, float throttle);

    ControlMode mode;
} Controller;

Controller* controller_create();

// In case we use multithreading
void controller_run();

void controller_update(Controller* controller);

void controller_destroy(Controller* controller);
