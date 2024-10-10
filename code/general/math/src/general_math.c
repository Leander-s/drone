#include <general_math.h>

void rotate_point_2D(const float angle, const float x, const float y, float *dstX, float *dstY){
    float rad = deg_to_rad(angle);
    *dstX = x * cos(rad) - y * sin(rad);
    *dstY = y * cos(rad) + x * sin(rad);
}

int clamp_int(int x, int A, int B){
    if(x > B) return B;
    if(x < A) return A;
    return x;
}

float clamp_float(float x, float A, float B){
    if(x > B) return B;
    if(x < A) return A;
    return x;
}

float deg_to_rad(float deg) { return deg * M_PI / 180; }

float rad_to_deg(float rad) { return rad * 180 / M_PI; }

void quaternion_set(Quaternion *quat, const vec3 *axis, const float degrees) {
  float angle = deg_to_rad(degrees);
  quat->x = cos(angle / 2);
  quat->i = sin(angle / 2) * axis->x;
  quat->j = sin(angle / 2) * axis->y;
  quat->k = sin(angle / 2) * axis->z;
}

float quaternion_magnitude(const Quaternion *quat) {
  return sqrt(pow(quat->x, 2) + pow(quat->i, 2) + pow(quat->j,2) + pow(quat->k, 2));
}

void quaternion_normalize(Quaternion *quat) {
  float magnitude = quaternion_magnitude(quat);
  quat->i /= magnitude;
  quat->j /= magnitude;
  quat->k /= magnitude;
  quat->x /= magnitude;
}

void mult_vec3_scalar(const vec3 *vec, const float x, vec3 *dstVec) {
  dstVec->x = vec->x * x;
  dstVec->y = vec->y * x;
  dstVec->z = vec->z * x;
}

void vec3_add(const vec3 *vec1, const vec3 *vec2, vec3 *dstVec) {
  dstVec->x = vec1->x + vec2->x;
  dstVec->y = vec1->y + vec2->y;
  dstVec->z = vec1->z + vec2->z;
}

float vec3_dot(const vec3 *vec1, const vec3 *vec2) {
  float result = 0;
  for (int i = 0; i < 3; i++) {
    result += vec1->data[i] * vec2->data[i];
  }
  return result;
}

void vec3_cross(const vec3 *vec1, const vec3 *vec2, vec3 *dstVec) {
  dstVec->x = (vec1->y * vec2->z - vec1->z * vec2->y);
  dstVec->y = (vec1->z * vec2->x - vec1->x * vec2->z);
  dstVec->z = (vec1->x * vec2->y - vec1->y * vec2->x);
}

void mult_quat_quat(const Quaternion *quat1, const Quaternion *quat2,
                    Quaternion *result) {
    /*
  vec3 axisPart1;
  mult_vec3_scalar(&quat1->v, quat2->w, &axisPart1);
  vec3 axisPart2;
  mult_vec3_scalar(&quat2->v, quat1->w, &axisPart2);
  vec3 axisPart3;
  vec3_add(&axisPart1, &axisPart2, &axisPart3);
  vec3 axisPart4;
  vec3_cross(&quat1->v, &quat2->v, &axisPart4);
  vec3 newV;
  vec3_add(&axisPart3, &axisPart4, &newV);
  float newW = quat1->w * quat2->w - vec3_dot(&quat2->v, &quat1->v);
  result->v = newV;
  result->w = newW;
  */

  result->x = quat1->x * quat2->x - quat1->i * quat2->i - quat1->j * quat2->j - quat1->k * quat2->k;
  result->i = quat1->x * quat2->i + quat1->i * quat2->x + quat1->j * quat2->k - quat1->k * quat2->j;
  result->j = quat1->x * quat2->j - quat1->i * quat2->k + quat1->j * quat2->x + quat1->k * quat2->i;
  result->k = quat1->x * quat2->k + quat1->i * quat2->j - quat1->j * quat2->i + quat1->k * quat2->x;
  quaternion_normalize(result);
}

void mult_quat_vec(const Quaternion *quat, const vec3 *vec,
                   Quaternion *result) {
  Quaternion vecQ;
  vecQ.v = *vec;
  vecQ.w = 0;
  mult_quat_quat(quat, &vecQ, result);
}

void mult_vec_quat(const vec3 *vec, const Quaternion *quat,
                   Quaternion *result) {
  Quaternion vecQ;
  vecQ.v = *vec;
  vecQ.w = 0;
  mult_quat_quat(&vecQ, quat, result);
}

void rotate_point(const Quaternion *quat, const vec3 *point, vec3 *dstPoint) {
  if (point->x == 0 && point->y == 0 && point->z == 0) {
    *dstPoint = *point;
    return;
  }

  Quaternion q = *quat;
  quaternion_normalize(&q);
  float magnitude = quaternion_magnitude(quat);
  Quaternion iq = (Quaternion){.i = -q.i, .j = -q.j, .k = -q.k, .x = q.x};

  Quaternion qp;
  mult_quat_vec(&q, point, &qp);
  Quaternion rotation;
  mult_quat_quat(&qp, &iq, &rotation);

  *dstPoint = rotation.v;
}
