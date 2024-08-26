#include "gui_math.h"

float deg_to_rad(float deg) { return deg * M_PI / 180; }

float rad_to_deg(float rad) { return rad * 180 / M_PI; }

void set_quaternion(Quaternion *quat, const vec3 *axis, const float degrees) {
  float angle = deg_to_rad(degrees);
  quat->x = cos(angle / 2);
  quat->i = sin(angle / 2) * axis->x;
  quat->j = sin(angle / 2) * axis->y;
  quat->k = sin(angle / 2) * axis->z;
}

float quaternion_magnitude(const Quaternion *quat) {
  return sqrt(pow((quat->x + quat->i + quat->j + quat->k), 2));
}

Quaternion quaternion_normalize(const Quaternion *quat) {
  float magnitude = quaternion_magnitude(quat);
  Quaternion result = (Quaternion){.i = quat->i / magnitude,
                                   .j = quat->j / magnitude,
                                   .k = quat->k / magnitude,
                                   .x = 0};
  return result;
}

vec2 translate_point(const mat4 *mvp, const mat4 *viewPort, const vec3 *point,
                     const float zoom) {
  vec4 pos =
      (vec4){.x = point->x, .y = point->y, .z = point->z + zoom, .w = 1.0f};
  vec4 clipPos = mult_mat_vec(mvp, &pos);
  vec4 ndcPos = (vec4){.x = clipPos.x / clipPos.w,
                       .y = clipPos.y / clipPos.w,
                       .z = clipPos.z / clipPos.w,
                       .w = 1.0f};

  vec4 screenPos4 = mult_mat_vec(viewPort, &ndcPos);
  return (vec2){.x = screenPos4.x, .y = screenPos4.y};
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

vec3 mult_vec3_scalar(const vec3 *vec, const float x) {
  vec3 result = (vec3){.x = vec->x * x, .y = vec->y * x, .z = vec->z * x};
  return result;
}

vec3 vec3_add(const vec3 *vec1, const vec3 *vec2) {
  vec3 result = (vec3){
      .x = vec1->x + vec2->x,
      .y = vec1->y + vec2->y,
      .z = vec1->z + vec2->z,
  };
  return result;
}

float vec3_dot(const vec3 *vec1, const vec3 *vec2) {
  float result = 0;
  for (int i = 0; i < 3; i++) {
    result += vec1->data[i] * vec2->data[i];
  }
  return result;
}

vec3 vec3_cross(const vec3 *vec1, const vec3 *vec2) {
  vec3 result = (vec3){.x = (vec1->y * vec2->z - vec1->z * vec2->y),
                       .y = (vec1->z * vec2->x - vec1->x * vec2->z),
                       .z = (vec1->x * vec2->y - vec1->y * vec2->x)};
  return result;
}

Quaternion mult_quat_quat(const Quaternion *quat1, const Quaternion *quat2) {
  // terrible memory wise but readable, maybe should change this to be efficient
  Quaternion result;
  vec3 axisPart1 = mult_vec3_scalar(&quat1->v, quat2->w);
  vec3 axisPart2 = mult_vec3_scalar(&quat2->v, quat1->w);
  vec3 axisPart3 = vec3_add(&axisPart1, &axisPart2);
  vec3 axisPart4 = vec3_cross(&quat1->v, &quat2->v);
  vec3 newV = vec3_add(&axisPart3, &axisPart4);
  float newW = quat1->w * quat2->w - vec3_dot(&quat2->v, &quat1->v);
  result.v = newV;
  result.w = newW;
  return result;
}

Quaternion mult_quat_vec(const Quaternion *quat, const vec3 *vec) {}

Quaternion mult_vec_quat(const vec3 *vec, const Quaternion *quat) {}

vec3 rotate_point(const Quaternion *quat, const vec3 *point) {
  // later
  Quaternion q = quaternion_normalize(quat);
  float magnitude = quaternion_magnitude(quat);
  Quaternion iq = (Quaternion){.i = -q.i, .j = -q.j, .k = -q.k, .x = 0};
}
