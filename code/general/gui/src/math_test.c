#include "gui_math.h"
#include <stdio.h>

void print_vec2(vec2 *vec) {
  printf("[");
  for (int i = 0; i < 2; i++) {
    printf("%f, ", vec->data[i]);
  }
  printf("]\n");
}

void print_vec4(vec4 *vec) {
  printf("[");
  for (int i = 0; i < 4; i++) {
    printf("%f, ", vec->data[i]);
  }
  printf("]\n");
}

void print_mat(mat4 *mat) {
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%f, ", mat->col[j].data[i]);
    }
    printf("]\n");
  }
}

void test_mat(const char *testName, mat4 *result, mat4 *expected) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (result->col[i].data[j] != expected->col[i].data[j]) {
        printf("%s failed. Mats:\n", testName);
        printf("Expected:\n");
        print_mat(expected);
        printf("Got:\n");
        print_mat(result);
        return;
      }
    }
  }
  printf("%s passed!\n", testName);
}

void test_vec2(const char *testName, vec2 *result, vec2 *expected) {
  for (int i = 0; i < 2; i++) {
    if (result->data[i] != expected->data[i]) {
      printf("%s failed. Vecs:\n", testName);
      printf("Expected:\n");
      print_vec2(expected);
      printf("Got:\n");
      print_vec2(result);
      return;
    }
  }
  printf("%s passed!\n", testName);
}

void test_vec4(const char *testName, vec4 *result, vec4 *expected) {
  for (int i = 0; i < 4; i++) {
    if (result->data[i] != expected->data[i]) {
      printf("%s failed. Vecs:\n", testName);
      printf("Expected:\n");
      print_vec4(expected);
      printf("Got:\n");
      print_vec4(result);
      return;
    }
  }
  printf("%s passed!\n", testName);
}

int main() {
  mat4 unity;
  unity.col[0] = (vec4){.x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0};
  unity.col[1] = (vec4){.x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0};
  unity.col[2] = (vec4){.x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0};
  unity.col[3] = (vec4){.x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0};

  mat4 matA;
  matA.col[0] = (vec4){.x = 1.0, .y = 2.0, .z = 3.0, .w = 4.0};
  matA.col[1] = (vec4){.x = 3.0, .y = 2.0, .z = 1.0, .w = 3.0};
  matA.col[2] = (vec4){.x = 1.0, .y = 2.0, .z = 1.0, .w = 3.0};
  matA.col[3] = (vec4){.x = 1.0, .y = 4.0, .z = 5.0, .w = 5.0};

  mat4 matB;
  matB.col[0] = (vec4){.x = 4.0, .y = 5.0, .z = 6.0, .w = 8.0};
  matB.col[1] = (vec4){.x = 6.0, .y = 1.0, .z = 5.0, .w = 4.0};
  matB.col[2] = (vec4){.x = 4.0, .y = 0.0, .z = 6.0, .w = 5.0};
  matB.col[3] = (vec4){.x = 1.0, .y = 9.0, .z = 1.0, .w = 0.0};

  mat4 controlMat;
  controlMat.col[0] = (vec4){.x = 32.0, .y = 43.0, .z = 38.0, .w = 31.0};
  controlMat.col[1] = (vec4){.x = 31.0, .y = 44.0, .z = 37.0, .w = 37.0};
  controlMat.col[2] = (vec4){.x = 23.0, .y = 34.0, .z = 25.0, .w = 21.0};
  controlMat.col[3] = (vec4){.x = 53.0, .y = 54.0, .z = 61.0, .w = 49.0};

  mat4 resultMat;

  mult_mat_mat(&matA, &matB, &resultMat);
  
  test_mat("Matrix multiplication test", &resultMat, &controlMat);

  vec4 testVec = (vec4){.x = 7.0, .y = 42.0, .z = 1337.0, .w = 3.0};

  vec4 result = mult_mat_vec(&unity, &testVec);

  test_vec4("Unity multiplication", &result, &testVec);

  vec3 testPoint = (vec3){.x = 0, .y = 0, .z = 10};

  mat4 mvp, viewPort;
  create_mvp(16.0f/9.0f, M_PI/2.0f, 100, 0.1, &mvp);
  create_view_port(1920, 1080, 100, 0.1, &viewPort);

  vec2 translatedPoint = translate_point(&mvp, &viewPort, &testPoint);
  vec2 controlPoint = (vec2){.x = 1920.0f/2.0f, .y = 1080.0f/2.0f};

  test_vec2("Point translation test", &translatedPoint, &controlPoint);
  return 0;
}
