#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

typedef union {
  struct {
    float x;
    float y;
  };
  float data[4];
} vec2;

typedef union {
  struct {
    float x;
    float y;
    float z;
  };
  float data[3];
} vec3;

typedef union {
  struct {
    float x;
    float y;
    float z;
    float w;
  };
  float data[4];
} vec4;

typedef struct {
  vec4 col[4];
} mat4;

typedef union {
  struct {
    float x;
    float i;
    float j;
    float k;
  };
  struct {
    float w;
    vec3 v;
  };
  float data[4];
} Quaternion;


// Controls
float approximate(float current, float target, float max);

// util
float max(float a, float b);

float min(float a, float b);

float float_abs(float x);

float deg_to_rad(float deg);

float rad_to_deg(float rad);

// Quaternion
void quaternion_set(Quaternion* result, const vec3 *axis, const float degrees);

float quaternion_magnitude(const Quaternion *quat);

void quaternion_normalize(Quaternion *quat);

void mult_quat_quat(const Quaternion *quat1, const Quaternion *quat2,
        Quaternion *result);

void mult_quat_vec(const Quaternion *quat, const vec3 *vec, Quaternion *result);

void mult_vec_quat(const vec3 *vec, const Quaternion *quat, Quaternion *result);

void rotate_point(const Quaternion *quat, const vec3 *point, vec3 *dstPoint);

// Vectors/Matrices
void mult_vec3_scalar(const vec3 *vec, const float x, vec3 *dstVec);

void vec3_add(const vec3 *vec1, const vec3 *vec2, vec3 *dstVec);

float vec3_dot(const vec3 *vec1, const vec3 *vec2);

void vec3_cross(const vec3 *vec1, const vec3 *vec2, vec3 *dstVec);
