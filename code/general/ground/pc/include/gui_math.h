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

typedef struct {
  // Vertex and index array or constant since we only need the drone model
  // for now, i test with a cube
  vec3 vertices[8];
  int indices[24];
} Model;

void quaternion_set(Quaternion* result, const vec3 *axis, const float degrees);

float quaternion_magnitude(const Quaternion *quat);

void quaternion_normalize(Quaternion *quat);

float deg_to_rad(float deg);

float rad_to_deg(float rad);

void translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point,
                     const float zoom, vec2 *dstPoint);

void create_mvp(const float aspectRatio, const float fov, const float far,
                const float near, mat4 *result);

void create_view_port(const float width, const float height, const float far,
                      const float near, mat4 *result);

void mult_mat_vec(const mat4 *mat, const vec4 *vec, vec4 *dstVec);

void mult_mat_mat(const mat4 *first, const mat4 *second, mat4 *result);

void mult_vec3_scalar(const vec3 *vec, const float x, vec3 *dstVec);

void vec3_add(const vec3 *vec1, const vec3 *vec2, vec3 *dstVec);

float vec3_dot(const vec3 *vec1, const vec3 *vec2);

void vec3_cross(const vec3 *vec1, const vec3 *vec2, vec3 *dstVec);

void mult_quat_quat(const Quaternion *quat1, const Quaternion *quat2,
                    Quaternion *result);

void mult_quat_vec(const Quaternion *quat, const vec3 *vec, Quaternion *result);

void mult_vec_quat(const vec3 *vec, const Quaternion *quat, Quaternion *result);

void rotate_point(const Quaternion *quat, const vec3 *point, vec3 *dstPoint);
