#include "gui_math.h"

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

vec4 mult_mat_vec(const mat4 *mat, vec4 *vec){
    vec4 result;
    memset(result.data, 0.0, 4 * sizeof(float));
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            float temp = mat->col[j].data[i] * vec->data[j];
            result.data[i] = result.data[i] + temp;
        }
    }
    return result;
}

vec3 rotate_point(const Quaternion *quat, const vec3 *point){

}
