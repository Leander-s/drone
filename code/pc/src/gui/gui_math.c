#include <cglm/mat4.h>
#include <gui_math.h>

void translate_point(const matrix4 *mvp, const matrix4 *viewPort, const vector3 *point,
                     const float zoom, vector2 *dstPoint) {
  vector4 pos =
      (vector4){.x = point->x + zoom, .y = point->y, .z = point->z, .w = 1.0f};
  vector4 clipPos;
  mult_mat_vec(mvp, &pos, &clipPos);
  vector4 ndcPos = (vector4){.x = clipPos.x / clipPos.w,
                       .y = clipPos.y / clipPos.w,
                       .z = clipPos.z / clipPos.w,
                       .w = 1.0f};

  vector4 screenPos4;
  mult_mat_vec(viewPort, &ndcPos, &screenPos4);
  dstPoint->x = screenPos4.x;
  dstPoint->y = screenPos4.y;
}

void create_mvp(const float aspectRatio, const float fov, const float near,
                const float far, mat4 mvp) {
  mat4 view, model, projection;

  glm_mat4_identity(model);
  glm_translate(model, (vec3){0.0f, 0.0f, -1.0f});
  //glm_rotate(model, glm_rad(0.0f), (vec3){0.0f, 1.0f, 0.0f});
  glm_scale(model, (vec3){1.0f, 1.0f, 1.0f});

  vec3 eye = {0.0f, 0.0f, 0.5f};
  vec3 center = {0.0f, 0.0f, 0.0f};
  vec3 up = {0.0f, 1.0f, 0.0f};
  glm_lookat(eye, center, up, view);

  glm_perspective(fov, aspectRatio, near, far, projection);

  glm_mat4_mul(projection, view, mvp);
  glm_mat4_mul(mvp, model, mvp);
}

void create_view_port(const float width, const float height, const float far,
                      const float near, matrix4 *result) {
  (*result) = (matrix4){
      .col[0] = (vector4){.x = width / 2, .y = 0.0f, .z = 0.0f, .w = 0.0f},
      .col[1] = (vector4){.x = 0.0f, .y = height / 2, .z = 0.0f, .w = 0.0f},
      .col[2] = (vector4){.x = 0.0f, .y = 0.0f, .z = (far - near) / 2, .w = 0.0f},
      .col[3] = (vector4){.x = width / 2,
                       .y = height / 2,
                       .z = (far + near) / 2,
                       .w = 0.0f},
  };
}

void mult_mat_mat(const matrix4 *first, const matrix4 *second, matrix4 *result) {
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

void mult_mat_vec(const matrix4 *mat, const vector4 *vec, vector4 *dstVec) {
  memset(dstVec->data, 0.0f, 4 * sizeof(float));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float temp = mat->col[j].data[i] * vec->data[j];
      dstVec->data[i] = dstVec->data[i] + temp;
    }
  }
}
