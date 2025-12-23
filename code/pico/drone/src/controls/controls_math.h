#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <general_math.h>

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
    float w;
  };
  float data[4];
} vec4;

typedef struct {
  vec4 col[4];
} mat4;

// Controls
float approximate(float current, float target, float max);

// util
float max(float a, float b);

float min(float a, float b);

float float_abs(float x);
