#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <general_math.h>

typedef union {
  struct {
    float x;
    float y;
  };
  float data[2];
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

typedef struct {
  // Vertex and index array or constant since we only need the drone model
  // for now, i test with a cube
  vec3 vertices[8];
  int indices[48];
} Model;

void translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point,
                     const float zoom, vec2 *dstPoint);

void create_mvp(const float aspectRatio, const float fov, const float far,
                const float near, mat4 *result);

void create_view_port(const float width, const float height, const float far,
                      const float near, mat4 *result);

void mult_mat_vec(const mat4 *mat, const vec4 *vec, vec4 *dstVec);

void mult_mat_mat(const mat4 *first, const mat4 *second, mat4 *result);
