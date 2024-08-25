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
  float data[4];
} Quaternion;

vec2 translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point);

vec4 mult_mat_vec(const mat4 *mat, vec4 *vec);

void mult_mat_mat(const mat4 *first, const mat4 *second, mat4 *result);

vec3 rotate_point(const Quaternion *quat, const vec3 *point);
