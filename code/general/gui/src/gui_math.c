#include "gui_math.h"

float deg_to_rad(float deg){
    return deg * M_PI/180;
}

float rad_to_deg(float rad){
    return rad * 180/M_PI;
}

vec2 translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point) {
  vec4 pos = (vec4){.x = point->x, .y = point->y, .z = point->z, .w = 1.0f};
  vec4 clipPos = mult_mat_vec(mvp, &pos);
  vec4 ndcPos = (vec4){.x = clipPos.x / clipPos.w,
                       .y = clipPos.y / clipPos.w,
                       .z = clipPos.z / clipPos.w,
                       .w = 1.0f};

  vec4 screenPos4 = mult_mat_vec(viewPort, &ndcPos);
  return (vec2){.x = screenPos4.x, .y = screenPos4.y};
}

void create_mvp(const float aspectRatio, const float fov, const float near, const float far, mat4 *result) {
  mat4 view, perspective;
  view = (mat4){
      .col[0] = (vec4){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f},
      .col[1] = (vec4){.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f},
      .col[2] = (vec4){.x = 0.0f, .y = 0.0f, .z = 1.0f, .w = 0.0f},
      .col[3] = (vec4){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f},
  };
  perspective = (mat4){
      .col[0] = (vec4){.x = 1/(aspectRatio*tan(fov/2)), .y = 0.0f, .z = 0.0f, .w = 0.0f},
      .col[1] = (vec4){.x = 0.0f, .y = 1/tan(fov/2), .z = 0.0f, .w = 0.0f},
      .col[2] = (vec4){.x = 0.0f, .y = 0.0f, .z = -(far+near)/(far-near), .w = -(2*far*near)/(far-near)},
      .col[3] = (vec4){.x = 0.0f, .y = 0.0f, .z = -1, .w = 0.0f},
  };
  mult_mat_mat(&view, &perspective, result);
}

void create_view_port(const float width, const float height, const float far, const float near, mat4 *result){
    (*result) = (mat4){
        .col[0] = (vec4){.x = width/2, .y = 0.0f, .z = 0.0f, .w = 0.0f},
        .col[1] = (vec4){.x = 0.0f, .y = height/2, .z = 0.0f, .w = 0.0f},
        .col[2] = (vec4){.x = 0.0f, .y = 0.0f, .z = (far-near)/2, .w = 0.0f},
        .col[3] = (vec4){.x = width/2, .y = height/2, .z = (far+near)/2, .w = 0.0f},
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

vec4 mult_mat_vec(const mat4 *mat, vec4 *vec) {
  vec4 result;
  memset(result.data, 0.0, 4 * sizeof(float));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float temp = mat->col[j].data[i] * vec->data[j];
      result.data[i] = result.data[i] + temp;
    }
  }
  return result;
}

vec3 rotate_point(const Quaternion *quat, const vec3 *point) {
  // later
}
