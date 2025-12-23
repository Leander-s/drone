#define _USE_MATH_DEFINES
#include <general_math.h>
#include <cglm/cglm.h>
#include <string.h>

typedef union {
  struct {
    float x;
    float y;
  };
  float data[2];
} vector2;

typedef union {
  struct {
    float x;
    float y;
    float z;
    float w;
  };
  float data[4];
} vector4;

typedef struct {
  vector4 col[4];
} matrix4;

typedef struct {
  // Vertex and index array or constant since we only need the drone model
  // for now, i test with a cube
  float *vertices;
  int *indices;
  int vertexCount;
  int indexCount;
  int vertexSize;
} Model;

void translate_point(const matrix4 *mvp, const matrix4 *viewPort, const vector3 *point,
                     const float zoom, vector2 *dstPoint);

void create_mvp(const float aspectRatio, const float fov, const float far,
                const float near, mat4 result);

void create_view_port(const float width, const float height, const float far,
                      const float near, matrix4 *result);

void mult_mat_vec(const matrix4 *mat, const vector4 *vec, vector4 *dstVec);

void mult_mat_mat(const matrix4 *first, const matrix4 *second, matrix4 *result);
