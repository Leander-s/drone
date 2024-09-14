#include <gui_math.h>

void translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point,
                     const float zoom, vec2 *dstPoint) {
  Quaternion littleTestRotation;
  vec3 randomAxis = (vec3){.x = 0.2, .y = 0.6, .z = 0.5};
  quaternion_set(&littleTestRotation, &randomAxis, 30);
  vec3 rotatedPoint;
  rotate_point(&littleTestRotation, point, &rotatedPoint);
  vec4 pos =
      (vec4){.x = rotatedPoint.x, .y = rotatedPoint.y, .z = rotatedPoint.z + zoom, .w = 1.0f};
  vec4 clipPos;
  mult_mat_vec(mvp, &pos, &clipPos);
  vec4 ndcPos = (vec4){.x = clipPos.x / clipPos.w,
                       .y = clipPos.y / clipPos.w,
                       .z = clipPos.z / clipPos.w,
                       .w = 1.0f};

  vec4 screenPos4;
  mult_mat_vec(viewPort, &ndcPos, &screenPos4);
  dstPoint->x = screenPos4.x;
  dstPoint->y = screenPos4.y;
}

void create_mvp(const float aspectRatio, const float fov, const float near,
                const float far, mat4 *result) {
  mat4 view, projection;
  view = (mat4){
      .col[0] = (vec4){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f},
      .col[1] = (vec4){.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f},
      .col[2] = (vec4){.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f},
      .col[3] = (vec4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
  };
  projection = (mat4){
      .col[0] = (vec4){.x = 1 / (aspectRatio * tan(fov / 2)),
                       .y = 0.0f,
                       .z = 0.0f,
                       .w = 0.0f},
      .col[1] = (vec4){.x = 0.0f, .y = 1 / tan(fov / 2), .z = 0.0f, .w = 0.0f},
      .col[2] = (vec4){.x = 0.0f,
                       .y = 0.0f,
                       .z = -(far + near) / (far - near),
                       .w = -(2 * far * near) / (far - near)},
      .col[3] = (vec4){.x = 0.0f, .y = 0.0f, .z = -1, .w = 0.0f},
  };
  mult_mat_mat(&projection, &view, result);
}

void create_view_port(const float width, const float height, const float far,
                      const float near, mat4 *result) {
  (*result) = (mat4){
      .col[0] = (vec4){.x = width / 2, .y = 0.0f, .z = 0.0f, .w = 0.0f},
      .col[1] = (vec4){.x = 0.0f, .y = height / 2, .z = 0.0f, .w = 0.0f},
      .col[2] = (vec4){.x = 0.0f, .y = 0.0f, .z = (far - near) / 2, .w = 0.0f},
      .col[3] = (vec4){.x = width / 2,
                       .y = height / 2,
                       .z = (far + near) / 2,
                       .w = 0.0f},
  };
}

void mult_mat_mat(const mat4 *first, const mat4 *second, mat4 *result) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float *current_point = &result->col[i].data[j];
      (*current_point) = 0;
      for (int k = 0; k < 4; k++) {
        (*current_point) += first->col[i].data[k] * second->col[k].data[j];
      }
    }
  }
}

void mult_mat_vec(const mat4 *mat, const vec4 *vec, vec4 *dstVec) {
  memset(dstVec->data, 0.0, 4 * sizeof(float));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float temp = mat->col[j].data[i] * vec->data[j];
      dstVec->data[i] = dstVec->data[i] + temp;
    }
  }
}
