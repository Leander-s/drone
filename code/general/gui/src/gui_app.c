#include "GL/glew.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <GL/glu.h>
#include <colors.h>
#include <gui_app.h>

char *readShaderSource(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Unable to open shader file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  rewind(fp);
  char *source = malloc(size + 1);
  fread(source, 1, size, fp);
  source[size] = '\0';
  fclose(fp);
  return source;
}

void setupBuffers(GUI *gui) {
  glGenVertexArrays(1, &gui->VAO_3D);
  glGenBuffers(1, &gui->VBO_3D);
  glGenBuffers(1, &gui->IBO_3D);

  glGenVertexArrays(1, &gui->VAO_text);
  glGenBuffers(1, &gui->VBO_text);
  glGenBuffers(1, &gui->IBO_text);

  glGenVertexArrays(1, &gui->VAO_HUD_rects);
  glGenBuffers(1, &gui->VBO_HUD_rects);
  glGenBuffers(1, &gui->IBO_HUD_rects);

  // 3D
  glBindVertexArray(gui->VAO_3D);

  glBindBuffer(GL_ARRAY_BUFFER, gui->VBO_3D);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gui->droneModel->vertices),
               gui->droneModel->vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui->IBO_3D);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gui->droneModel->indices),
               gui->droneModel->indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /*
  // color attribute
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
          (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  */

  // text
  glBindVertexArray(gui->VAO_text);

  glBindBuffer(GL_ARRAY_BUFFER, gui->VBO_text);

  float vertices[] = {
      // x, y,   u, v
      0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
      1.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
      1.0f, 1.0f, 1.0f, 0.0f, // Top-right

      0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
      1.0f, 1.0f, 1.0f, 0.0f, // Top-right
      0.0f, 1.0f, 0.0f, 0.0f  // Top-left
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // texcoord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // HUD
  glBindVertexArray(gui->VAO_HUD_rects);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // HUD
  glBindVertexArray(gui->VAO_HUD_lines);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

GLuint compileShader(const char *path, GLenum type) {
  char *src = readShaderSource(path);
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar **)&src, NULL);
  glCompileShader(shader);
  free(src);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char log[512];
    glGetShaderInfoLog(shader, 512, NULL, log);
    fprintf(stderr, "Shader compile error (%s):\n%s\n", path, log);
    exit(EXIT_FAILURE);
  }
  return shader;
}

GLuint setupShaders(const char *vertPath, const char *fragPath) {
  GLuint vertexShader = compileShader(vertPath, GL_VERTEX_SHADER);
  GLuint fragmentShader = compileShader(fragPath, GL_FRAGMENT_SHADER);

  GLuint shaderProgram_3D = glCreateProgram();
  glAttachShader(shaderProgram_3D, vertexShader);
  glAttachShader(shaderProgram_3D, fragmentShader);
  glLinkProgram(shaderProgram_3D);

  GLint success;
  glGetProgramiv(shaderProgram_3D, GL_LINK_STATUS, &success);
  if (!success) {
    char log[512];
    glGetProgramInfoLog(shaderProgram_3D, 512, NULL, log);
    fprintf(stderr, "Shader linking error:\n%s\n", log);
    exit(EXIT_FAILURE);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return shaderProgram_3D;
}

GUI *gui_create(int width, int height) {
  int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
  if (result != 0) {
    SDL_Log("SDL_Init failed : %s\n", SDL_GetError());
    return NULL;
  }

  result = TTF_Init();
  if (result != 0) {
    printf("Error initializing ttf. Text cannot be rendered. Error : %s\n",
           TTF_GetError());
  }

  GUI *gui = malloc(sizeof(GUI));
  if (!gui) {
    SDL_Log("Failed to allocate memory for GUI");
    return NULL;
  }

  gui->width = width;
  gui->height = height;
  gui->shouldQuit = 0;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

  gui->window = SDL_CreateWindow("Drone Controller", width, height,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!gui->window) {
    SDL_Log("Creating window failed : %s\n", SDL_GetError());
    return NULL;
  }

  printf("Window and renderer created\n");

  gui->glContext = SDL_GL_CreateContext(gui->window);
  if (!gui->glContext) {
    SDL_Log("Creating context failed : %s\n", SDL_GetError());
    return NULL;
  }
  // Explicitly make context current (to be safe)
  if (SDL_GL_MakeCurrent(gui->window, gui->glContext) != 0) {
    fprintf(stderr, "SDL_GL_MakeCurrent Error: %s\n", SDL_GetError());
    SDL_GL_DestroyContext(gui->glContext);
    SDL_DestroyWindow(gui->window);
    SDL_Quit();
    return NULL;
  }

  printf("glContext created\n");

  const GLubyte *version = glGetString(GL_VERSION);
  if (!version) {
    fprintf(stderr, "No valid OpenGL context!\n");
    SDL_GL_DestroyContext(gui->glContext);
    SDL_DestroyWindow(gui->window);
    SDL_Quit();
    return NULL;
  }
  printf("OpenGL version: %s\n", version);

  glewExperimental = GL_TRUE;

  printf("Initilizing glew\n");
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    SDL_Log("GLEW initialization failed");
    return NULL;
  }

  printf("GLEW version: %s\n", glewGetString(GLEW_VERSION));
  if (GLEW_VERSION_3_3) {
    printf("OpenGL 3.3 is supported\n");
  } else {
    printf("OpenGL 3.3 is NOT supported\n");
  }

  gui->droneModel = drone_model_create();
  gui->hudModels = hud_models_create();

  glEnable(GL_DEPTH_TEST);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("Error in depth test: 0x%x\n", err);
  }

  gui->shaderProgram_3D =
      setupShaders("../shaders/shader.vert", "../shaders/shader.frag");
  gui->shaderProgram_text =
      setupShaders("../shaders/text.vert", "../shaders/text.frag");
  gui->shaderProgram_HUD_rects =
      setupShaders("../shaders/hud_rects.vert", "../shaders/hud_rects.frag");
  gui->shaderProgram_HUD_lines=
      setupShaders("../shaders/hud_lines.vert", "../shaders/hud_lines.frag");

  setupBuffers(gui);

  gui->updateCounter = 0;
  memset(gui->keyState, 0, 322);

  return gui;
}

void gui_update(GUI *gui, const GUIData *data) {
  glClearColor(0.021f, 0.043f, 0.09f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, gui->width / 2, gui->height);
  // hud_draw(gui, data);
  drone_model_draw(gui, data);

  glViewport(gui->width / 2, 0, gui->width / 2, gui->height);
  data_sheet_draw(gui, data);

  SDL_GL_SwapWindow(gui->window);

  SDL_Event event;
  SDL_PollEvent(&event);

  SDL_Keymod currentModState = SDL_GetModState();

  if (gui->updateCounter == 0) {
    if (currentModState & SDL_KMOD_LSHIFT) {
      if (data->controlState->throttle != 255) {
        data->controlState->throttle++;
      }
    }
    if (currentModState & SDL_KMOD_LCTRL) {
      if (data->controlState->throttle != 0) {
        data->controlState->throttle--;
      }
    }
  }

  data->controlState->pitch = 127;
  data->controlState->roll = 127;
  data->controlState->yaw = 127;

  if (gui->keyState[SDLK_W]) {
    data->controlState->pitch -= 60;
  }
  if (gui->keyState[SDLK_S]) {
    data->controlState->pitch += 60;
  }
  if (gui->keyState[SDLK_A]) {
    data->controlState->roll -= 60;
  }
  if (gui->keyState[SDLK_D]) {
    data->controlState->roll += 60;
  }
  if (gui->keyState[SDLK_Q]) {
    data->controlState->yaw -= 60;
  }
  if (gui->keyState[SDLK_E]) {
    data->controlState->yaw += 60;
  }

  switch (event.type) {
  case SDL_EVENT_QUIT:
    printf("Quitting\n");
    gui->shouldQuit = 1;
    return;
  case SDL_EVENT_KEY_DOWN:
    if (event.key.key > 321) {
      break;
    }
    gui->keyState[event.key.key] = 1;
    break;
  case SDL_EVENT_KEY_UP:
    if (event.key.key > 321) {
      break;
    }
    gui->keyState[event.key.key] = 0;
    break;
  case SDL_EVENT_WINDOW_RESIZED:
    SDL_GetWindowSizeInPixels(gui->window, &gui->width, &gui->height);
    break;
  }

  gui->updateCounter = (gui->updateCounter + 1) % 1;
}

void gui_destroy(GUI *gui) {
  glDeleteBuffers(1, &gui->VBO_3D);
  glDeleteBuffers(1, &gui->VAO_3D);
  glDeleteBuffers(1, &gui->IBO_3D);
  glDeleteBuffers(1, &gui->VBO_HUD_rects);
  glDeleteBuffers(1, &gui->VAO_HUD_rects);
  glDeleteBuffers(1, &gui->IBO_HUD_rects);
  glDeleteBuffers(1, &gui->VBO_HUD_lines);
  glDeleteBuffers(1, &gui->VAO_HUD_lines);
  glDeleteBuffers(1, &gui->IBO_HUD_lines);
  glDeleteBuffers(1, &gui->VBO_text);
  glDeleteBuffers(1, &gui->VAO_text);
  glDeleteBuffers(1, &gui->IBO_text);
  model_destroy(gui->droneModel);
  for(int i = 0; i < 4; i++){
    model_destroy(gui->hudModels[i]);
  }
  SDL_GL_DestroyContext(gui->glContext);
  SDL_DestroyWindow(gui->window);
  SDL_Quit();
}

void hud_draw(GUI *gui, const GUIData *data) {
  SDL_Renderer *renderer = gui->renderer;
  const int hudX = 0;
  const int hudY = 0;
  const int hudWidth = gui->width / 2;
  const int hudHeight = gui->height;

  const int botLeftX = hudWidth / 2;
  const int botLeftY = hudHeight / 4 * 3;
  const int botLeftWidth = hudWidth - botLeftX;
  const int botLeftHeight = hudHeight - botLeftY;

  const int botRightX = 0;
  const int botRightY = hudHeight / 4 * 3;
  const int botRightWidth = hudWidth / 2;
  const int botRightHeight = hudHeight - botLeftY;

  // draw throttle
  const int throttleBoxX = botLeftX + botLeftWidth - 20;
  const int throttleBoxY = botLeftY;
  const int throttleBoxWidth = 10;
  const int throttleBoxHeight = botLeftHeight - 10;

  // draw box
  rect_draw(renderer, throttleBoxX, throttleBoxY, throttleBoxWidth,
            throttleBoxHeight, &WHITE);

  // draw throttle
  const int throttleHeight =
      (int)((float)(data->controlState->throttle) / 255 * throttleBoxHeight);

  rect_fill(renderer, throttleBoxX,
            throttleBoxY + throttleBoxHeight - throttleHeight, throttleBoxWidth,
            throttleHeight, &WHITE);

  // draw pitch/yaw

  // draw box
  int boxX = botRightX + 10;
  int boxY = botRightY;
  int boxWidth = botRightWidth / 2;
  int boxHeight = botRightHeight / 2 - 10;

  rect_draw(renderer, boxX, boxY, boxWidth, boxHeight, &WHITE);

  // draw crosshair
  line_draw(renderer, boxX, boxY + boxHeight / 2, boxX + boxWidth,
            boxY + boxHeight / 2, &WHITE);

  line_draw(renderer, boxX + boxWidth / 2, boxY, boxX + boxWidth / 2,
            boxY + boxHeight, &WHITE);

  // draw indicator
  int indX = boxX + (float)data->controlState->yaw / 255 * boxWidth;
  int indY = boxY + (float)(255 - data->controlState->pitch) / 255 * boxHeight;

  rect_fill(renderer, indX - 5, indY - 5, 10, 10, &WHITE);

  // draw roll

  int rollLineX = botRightX + botRightWidth / 4 + 10;
  int rollLineY = botRightY + boxHeight + boxHeight / 2 - 10;

  int rollLineLeftX = 10;
  int rollLineRightX = rollLineX + (rollLineX - rollLineLeftX);

  float angle = (float)(data->controlState->roll - 127) / 255 * 90;

  float startX, startY, endX, endY;

  rotate_point_2D(angle, rollLineLeftX - rollLineX, 0, &startX, &startY);
  rotate_point_2D(angle, rollLineRightX - rollLineX, 0, &endX, &endY);

  startX += rollLineX;
  startY += rollLineY;
  endX += rollLineX;
  endY += rollLineY;

  line_draw(renderer, startX, startY, endX, endY, &WHITE);
}

void data_sheet_draw(GUI *gui, const GUIData *data) {
  const int x = gui->width / 2;
  const int y = 0;
  const int width = gui->width - x;
  const int height = gui->height;

  const int padding = 20;
  const int textX = x + padding;
  int yOffset = padding;

  // rect_fill(renderer, x, y, width, height, &DARK_GREY);
  // rect_draw(renderer, x, y, width, height, &GREY);

  text_draw(gui, textX, yOffset, "System logs", 30, &LIGHT_GREY);
  yOffset += 30;

  // maybe abstract this away into a pure function that takes the data and
  // returns the text pointer
  char transmissionsBuffer[64];
  sprintf(transmissionsBuffer, "Transmissions per second: %f",
          data->log->transmissionsPerSecond);
  const char *transmissionsText = transmissionsBuffer;
  text_draw(gui, textX, yOffset, transmissionsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char picoTimeoutsBuffer[64];
  sprintf(picoTimeoutsBuffer, "Pico read timeouts: %d",
          data->log->picoReadTimeouts);
  const char *picoTimeoutsText = picoTimeoutsBuffer;
  text_draw(gui, textX, yOffset, picoTimeoutsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbDisconnectsBuffer[64];
  sprintf(usbDisconnectsBuffer, "USB disconnects: %d",
          data->log->usbDisconnects);
  const char *usbDisconnectsText = usbDisconnectsBuffer;
  text_draw(gui, textX, yOffset, usbDisconnectsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbReadErrorsBuffer[64];
  sprintf(usbReadErrorsBuffer, "USB read errors: %d", data->log->usbReadErrors);
  const char *usbReadErrorsText = usbReadErrorsBuffer;
  text_draw(gui, textX, yOffset, usbReadErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbWriteErrorsBuffer[64];
  sprintf(usbWriteErrorsBuffer, "USB write errors: %d",
          data->log->usbWriteErrors);
  const char *usbWriteErrorsText = usbWriteErrorsBuffer;
  text_draw(gui, textX, yOffset, usbWriteErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;
}

Model **hud_models_create(){
    Model **hud = malloc(4 * sizeof(Model*));

    // lines for pitch/yaw/throttle
    float lines_verts[] = {
        // Pitch/Yaw lines
        -0.8f, -0.6f, 1.0f, 1.0f, 1.0f, // bottom-left      0
        -0.6f, -0.6f, 1.0f, 1.0f, 1.0f, // bottom-right     1
        -0.6f, -0.4f, 1.0f, 1.0f, 1.0f, // top-right        2
        -0.8f, -0.4f, 1.0f, 1.0f, 1.0f, // top-left         3
        -0.7f, -0.4f, 1.0f, 1.0f, 1.0f, // top-center       4
        -0.7f, -0.6f, 1.0f, 1.0f, 1.0f, // bottom-center    5
        -0.8f, -0.5f, 1.0f, 1.0f, 1.0f, // left-center      6
        -0.6f, -0.5f, 1.0f, 1.0f, 1.0f, // right-center     7

        // Throttle lines
        0.8f, -0.8f, 1.0f, 1.0f, 1.0f,  // bottom-right     8
        0.7f, -0.8f, 1.0f, 1.0f, 1.0f,  // bottom-left      9
        0.7f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-left         10
        0.8f, 0.0f, 1.0f, 1.0f, 1.0f,   // bottom-left      11
    };

    int lines_indices[] = {
        // Pitch/Yaw lines
        0, 1, 1, 2, 2, 3, 3, 0, // outline
        4, 5, 6, 7,             // cross 

        // Throttle lines
        8, 9, 9, 10, 10, 11, 11, 8
    };

    hud[0] = malloc(sizeof(Model));
    hud[0]->vertexCount = sizeof(lines_verts) / sizeof(float);
    hud[0]->vertices = malloc(sizeof(lines_verts));
    memcpy(hud[0]->vertices, lines_verts, sizeof(lines_verts));

    hud[0]->indexCount = sizeof(lines_indices) / sizeof(float);
    hud[0]->indices = malloc(sizeof(lines_indices));
    memcpy(hud[0]->indices, lines_indices, sizeof(lines_indices));

    // indicator for pitch/yaw
    float py_ind_verts[] = {
        -0.05f, -0.05f, 1.0f, 1.0f, 1.0f,   // bottom-left      0
        0.05f, -0.05f, 1.0f, 1.0f, 1.0f,    // bottom-right     1
        0.05f, 0.05f, 1.0f, 1.0f, 1.0f,     // top-right        2
        -0.05f, 0.05f, 1.0f, 1.0f, 1.0f,    // top-left         3
    };

    int py_ind_indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    hud[1] = malloc(sizeof(Model));
    hud[1]->vertexCount = sizeof(py_ind_verts) / sizeof(float);
    hud[1]->vertices = malloc(sizeof(py_ind_verts));
    memcpy(hud[1]->vertices, py_ind_verts, sizeof(py_ind_verts));

    hud[1]->indexCount = sizeof(py_ind_indices) / sizeof(float);
    hud[1]->indices = malloc(sizeof(py_ind_indices));
    memcpy(hud[1]->indices, py_ind_indices, sizeof(py_ind_indices));

    // lines for roll
    float roll_verts[] = {
    };

    int roll_indices[] = {
    };

    // indicator for throttle
    float throttle_verts[] = {
    };

    int throttle_indices[] = {
    };

    return hud;
}

Model *drone_model_create() {
  Model *droneModel = malloc(sizeof(Model));
  float verts[] = {
      // right, bottom, front
      0.3f, -0.1f, -0.5f, 0.0f, -1.0f, 0.0f, // 0, bottom
      0.3f, -0.1f, -0.5f, 1.0f, 0.0f, 0.0f,  // 1, right
      0.3f, -0.1f, -0.5f, 0.0f, 0.0f, -1.0f, // 2, front

      // right, top, front
      0.3f, 0.1f, -0.5f, 0.0f, 1.0f, 0.0f,  // 3, top
      0.3f, 0.1f, -0.5f, 1.0f, 0.0f, 0.0f,  // 4, right
      0.3f, 0.1f, -0.5f, 0.0f, 0.0f, -1.0f, // 5, front

      // right, top, back
      0.3f, 0.1f, 0.5f, 0.0f, 1.0f, 0.0f, // 6, top
      0.3f, 0.1f, 0.5f, 1.0f, 0.0f, 0.0f, // 7, right
      0.3f, 0.1f, 0.5f, 0.0f, 0.0f, 1.0f, // 8, back

      // right, bottom, back
      0.3f, -0.1f, 0.5f, 0.0f, -1.0f, 0.0f, // 9, bottom
      0.3f, -0.1f, 0.5f, 1.0f, 0.0f, 0.0f,  // 10, right
      0.3f, -0.1f, 0.5f, 0.0f, 0.0f, 1.0f,  // 11, back

      // left, bottom, front
      -0.3f, -0.1f, -0.5f, 0.0f, -1.0f, 0.0f, // 12, bottom
      -0.3f, -0.1f, -0.5f, -1.0f, 0.0f, 0.0f, // 13, left
      -0.3f, -0.1f, -0.5f, 0.0f, 0.0f, -1.0f, // 14, front

      // left, top, front
      -0.3f, 0.1f, -0.5f, 0.0f, 1.0f, 0.0f,  // 15, top
      -0.3f, 0.1f, -0.5f, -1.0f, 0.0f, 0.0f, // 16, left
      -0.3f, 0.1f, -0.5f, 0.0f, 0.0f, -1.0f, // 17, front

      // left, top, back
      -0.3f, 0.1f, 0.5f, 0.0f, 1.0f, 0.0f,  // 18, top
      -0.3f, 0.1f, 0.5f, -1.0f, 0.0f, 0.0f, // 19, left
      -0.3f, 0.1f, 0.5f, 0.0f, 0.0f, 1.0f,  // 20, back

      // left, bottom, back
      -0.3f, -0.1f, 0.5f, 0.0f, -1.0f, 0.0f, // 21, bottom
      -0.3f, -0.1f, 0.5f, -1.0f, 0.0f, 0.0f, // 22, left
      -0.3f, -0.1f, 0.5f, 0.0f, 0.0f, 1.0f,  // 23, back
  };

  droneModel->vertexCount = sizeof(verts)/4;
  droneModel->vertices = malloc(sizeof(verts));
  memcpy(droneModel->vertices, verts, sizeof(verts));

  int indices[] = {
      0,  9,  12, 9,  21, 12, // bottom
      1,  7,  10, 1,  4,  7,  // right
      2,  14, 5,  14, 17, 5,  // front
      15, 6,  3,  15, 18, 6,  // top
      8,  20, 11, 11, 20, 23, // back
      13, 19, 16, 13, 22, 19, // left
  };

  droneModel->indexCount = sizeof(indices)/4;
  droneModel->indices = malloc(sizeof(indices));
  memcpy(droneModel->indices, indices, sizeof(indices));

  return droneModel;
}

void model_destroy(Model *model) { 
    free(model->vertices);
    free(model->indices);
    free(model); 
}

void drone_model_draw(GUI *gui, const GUIData *data) {
  glUseProgram(gui->shaderProgram_3D);

  Model *droneModel = gui->droneModel;

  mat4 projection;
  // mat4 projection, viewPort;
  // create_view_port(gui->width / 2.0f, gui->height, 100.0f, 0.1f, viewPort);
  create_mvp((float)gui->width / 2 / gui->height, deg_to_rad(60.0f), 100.0f,
             0.1f, projection);

  GLuint uOrientation =
      glGetUniformLocation(gui->shaderProgram_3D, "uOrientation");
  GLuint uProjection =
      glGetUniformLocation(gui->shaderProgram_3D, "uProjection");

  glUniform4f(uOrientation, data->sensorState->orientation.w,
              data->sensorState->orientation.i,
              data->sensorState->orientation.j,
              data->sensorState->orientation.k);

  glUniformMatrix4fv(uProjection, 1, GL_FALSE, (float *)projection);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glBindVertexArray(gui->VAO_3D);
  glDrawElements(GL_TRIANGLES, gui->droneModel->indexCount,
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void line_draw(SDL_Renderer *renderer, int x, int y, int endX, int endY,
               const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  SDL_RenderLine(renderer, x, y, endX, endY);
}

void rect_draw(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  SDL_FRect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_RenderRect(renderer, &rect);
}

void rect_fill(SDL_Renderer *renderer, int x, int y, int w, int h,
               const SDL_Color *color) {
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  SDL_FRect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_RenderFillRect(renderer, &rect);
}

void text_draw(GUI *gui, int x, int y, const char *text, int fontSize,
               const SDL_Color *color) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  TTF_Font *font = TTF_OpenFont("../fonts/Montserrat-Regular.ttf", fontSize);
  if (font == NULL) {
    printf("Error opening font : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Surface *incorrect_surface = TTF_RenderUTF8_Blended(font, text, *color);
  SDL_Surface *surface =
      SDL_ConvertSurface(incorrect_surface, SDL_PIXELFORMAT_ABGR8888);
  TTF_CloseFont(font);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, surface->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  int tex_w = surface->w;
  int tex_h = surface->h;

  SDL_DestroySurface(surface);

  mat4 projection;
  glm_ortho(gui->width / 2.0f, gui->width, 0.0f, gui->height, -1.0f, 1.0f,
            projection);

  glEnable(GL_TEXTURE_2D);

  glUseProgram(gui->shaderProgram_text);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  GLuint tex_loc = glGetUniformLocation(gui->shaderProgram_text, "uTexture");
  glUniform1i(tex_loc, 0);

  mat4 model;
  glm_mat4_identity(model);
  glm_translate(model, (vec3){(float)x, (float)(gui->height - 2 * y), 0.0f});
  glm_scale(model, (vec3){(float)tex_w, (float)tex_h, 1.0f});

  mat4 proj_model;
  GLint proj_loc = glGetUniformLocation(gui->shaderProgram_text, "uProjection");
  glm_mat4_mul(projection, model, proj_model);
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float *)proj_model);

  glBindVertexArray(gui->VAO_text);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glDeleteTextures(1, &texture);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}
