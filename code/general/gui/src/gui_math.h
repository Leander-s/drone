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
  float data[4];
} Quaternion;

typedef struct {
  // Vertex and index array or constant since we only need the drone model
  // for now, i test with a cube
  vec3 vertices[8];
  int indices[24];
} Model;

float quaternion_magnitude(const Quaternion *quat);

Quaternion quaternion_normalize(const Quaternion *quat);

float deg_to_rad(float deg);

float rad_to_deg(float rad);

vec2 translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point,
                     const float zoom);

void create_mvp(const float aspectRatio, const float fov, const float far,
                const float near, mat4 *result);

void create_view_port(const float width, const float height, const float far,
                      const float near, mat4 *result);

vec4 mult_mat_vec(const mat4 *mat, vec4 *vec);

void mult_mat_mat(const mat4 *first, const mat4 *second, mat4 *result);

vec3 mult_vec3_scalar(const vec3 *vec, const float x);

float vec3_dot(const vec3 *vec1, const vec3 *vec2);

vec3 vec3_cross(const vec3 *vec1, const vec3 *vec2);

Quaternion mult_quat_vec(const Quaternion *quat, const vec3 *vec);

Quaternion mult_vec_quat(const vec3 *vec, const Quaternion *quat);

vec3 rotate_point(const Quaternion *quat, const vec3 *point);
