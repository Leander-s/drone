#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

typedef union {
  struct {
    float x;
    float y;
    float z;
  };
  float data[3];
} vector3;

typedef union {
  struct {
    float x;
    float i;
    float j;
    float k;
  };
  struct {
    float w;
    vector3 v;
  };
  float data[4];
} Quaternion;

typedef struct {
    float roll;
    float pitch;
    float yaw;
} EulerAngles;

void quaternion_set(Quaternion *result, const vector3 *axis, const float degrees);

float quaternion_magnitude(const Quaternion *quat);

void quaternion_normalize(Quaternion *quat);

EulerAngles quaternion_to_euler(const Quaternion *quat);

float deg_to_rad(float deg);

float rad_to_deg(float rad);

int clamp_int(int x, int A, int B);

float clamp_float(float x, float A, float B);

void mult_vec3_scalar(const vector3 *vec, const float x, vector3 *dstVec);

void vec3_add(const vector3 *vec1, const vector3 *vec2, vector3 *dstVec);

float vec3_dot(const vector3 *vec1, const vector3 *vec2);

void vec3_cross(const vector3 *vec1, const vector3 *vec2, vector3 *dstVec);

void mult_quat_quat(const Quaternion *quat1, const Quaternion *quat2,
                    Quaternion *result);

void mult_quat_vec(const Quaternion *quat, const vector3 *vec, Quaternion *result);

void mult_vec_quat(const vector3 *vec, const Quaternion *quat, Quaternion *result);

void rotate_point(const Quaternion *quat, const vector3 *point, vector3 *dstPoint);

void rotate_point_2D(const float angle, const float x, const float y,
                     float *dstX, float *dstY);
