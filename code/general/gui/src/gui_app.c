#include "GL/glew.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
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
  glGenVertexArrays(1, &gui->VAO);
  glGenBuffers(1, &gui->VBO);
  glGenBuffers(1, &gui->IBO);

  glBindVertexArray(gui->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, gui->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gui->droneModel->vertices),
               gui->droneModel->vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui->IBO);
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
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 *
  sizeof(float))); glEnableVertexAttribArray(2);
  */
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

GLuint setupShaders() {
  GLuint vertexShader =
      compileShader("../shaders/shader.vert", GL_VERTEX_SHADER);
  GLuint fragmentShader =
      compileShader("../shaders/shader.frag", GL_FRAGMENT_SHADER);

  printf("Creating program");
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char log[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, log);
    fprintf(stderr, "Shader linking error:\n%s\n", log);
    exit(EXIT_FAILURE);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return shaderProgram;
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

  gui->window =
      SDL_CreateWindow("Drone Controller", width, height, SDL_WINDOW_OPENGL);
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

  glEnable(GL_DEPTH_TEST);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("Error in depth test: 0x%x\n", err);
  }

  gui->shaderProgram = setupShaders();

  setupBuffers(gui);

  gui->updateCounter = 0;
  memset(gui->keyState, 0, 322);

  return gui;
}

void gui_update(GUI *gui, const GUIData *data) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);

  // data_sheet_draw(gui, data);
  drone_model_draw(gui, data);
  // hud_draw(gui, data);
  
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
  glDeleteBuffers(1, &gui->VBO);
  glDeleteBuffers(1, &gui->VAO);
  glDeleteBuffers(1, &gui->IBO);
  drone_model_destroy(gui->droneModel);
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
  SDL_Renderer *renderer = gui->renderer;
  const int x = gui->width / 2;
  const int y = 0;
  const int width = gui->width - x;
  const int height = gui->height;

  const int padding = 20;
  const int textX = x + padding;
  int yOffset = padding;

  rect_fill(renderer, x, y, width, height, &DARK_GREY);
  rect_draw(renderer, x, y, width, height, &GREY);

  text_draw(renderer, textX, yOffset, "System logs", 30, &LIGHT_GREY);
  yOffset += 30;

  // maybe abstract this away into a pure function that takes the data and
  // returns the text pointer
  char transmissionsBuffer[64];
  sprintf(transmissionsBuffer, "Transmissions per second: %f",
          data->log->transmissionsPerSecond);
  const char *transmissionsText = transmissionsBuffer;
  text_draw(renderer, textX, yOffset, transmissionsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char picoTimeoutsBuffer[64];
  sprintf(picoTimeoutsBuffer, "Pico read timeouts: %d",
          data->log->picoReadTimeouts);
  const char *picoTimeoutsText = picoTimeoutsBuffer;
  text_draw(renderer, textX, yOffset, picoTimeoutsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbDisconnectsBuffer[64];
  sprintf(usbDisconnectsBuffer, "USB disconnects: %d",
          data->log->usbDisconnects);
  const char *usbDisconnectsText = usbDisconnectsBuffer;
  text_draw(renderer, textX, yOffset, usbDisconnectsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbReadErrorsBuffer[64];
  sprintf(usbReadErrorsBuffer, "USB read errors: %d", data->log->usbReadErrors);
  const char *usbReadErrorsText = usbReadErrorsBuffer;
  text_draw(renderer, textX, yOffset, usbReadErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;

  char usbWriteErrorsBuffer[64];
  sprintf(usbWriteErrorsBuffer, "USB write errors: %d",
          data->log->usbWriteErrors);
  const char *usbWriteErrorsText = usbWriteErrorsBuffer;
  text_draw(renderer, textX, yOffset, usbWriteErrorsText, 20, &LIGHT_GREY);
  yOffset += padding;
}

Model *drone_model_create() {
  Model *droneModel = malloc(sizeof(Model));
  float verts[8 * 3 * 6] = {
      0.5f,  -0.1f, -0.3f, 0.0f,  -1.0f, 0.0f,  // 0
      0.5f,  -0.1f, -0.3f, 1.0f,  0.0f,  0.0f,  // 1
      0.5f,  -0.1f, -0.3f, 0.0f,  0.0f,  -1.0f, // 2

      0.5f,  0.1f,  -0.3f, 0.0f,  1.0f,  0.0f,  // 3
      0.5f,  0.1f,  -0.3f, 1.0f,  0.0f,  0.0f,  // 4
      0.5f,  0.1f,  -0.3f, 0.0f,  0.0f,  -1.0f, // 5

      0.5f,  0.1f,  0.3f,  0.0f,  1.0f,  0.0f, // 6
      0.5f,  0.1f,  0.3f,  1.0f,  0.0f,  0.0f, // 7
      0.5f,  0.1f,  0.3f,  0.0f,  0.0f,  1.0f, // 8

      0.5f,  -0.1f, 0.3f,  0.0f,  -1.0f, 0.0f, // 9
      0.5f,  -0.1f, 0.3f,  1.0f,  0.0f,  0.0f, // 10
      0.5f,  -0.1f, 0.3f,  0.0f,  0.0f,  1.0f, // 11

      -0.5f, -0.1f, -0.3f, 0.0f,  -1.0f, 0.0f,  // 12
      -0.5f, -0.1f, -0.3f, -1.0f, 0.0f,  0.0f,  // 13
      -0.5f, -0.1f, -0.3f, 0.0f,  0.0f,  -1.0f, // 14

      -0.5f, 0.1f,  -0.3f, 0.0f,  1.0f,  0.0f,  // 15
      -0.5f, 0.1f,  -0.3f, -1.0f, 0.0f,  0.0f,  // 16
      -0.5f, 0.1f,  -0.3f, 0.0f,  0.0f,  -1.0f, // 17

      -0.5f, 0.1f,  0.3f,  0.0f,  1.0f,  0.0f, // 18
      -0.5f, 0.1f,  0.3f,  -1.0f, 0.0f,  0.0f, // 19
      -0.5f, 0.1f,  0.3f,  0.0f,  0.0f,  1.0f, // 20

      -0.5f, -0.1f, 0.3f,  0.0f,  -1.0f, 0.0f, // 21
      -0.5f, -0.1f, 0.3f,  -1.0f, 0.0f,  0.0f, // 22
      -0.5f, -0.1f, 0.3f,  0.0f,  0.0f,  1.0f, // 23
  };

  memcpy(droneModel->vertices, verts, 8 * 3 * 6 * 4);

  int indices[6 * 6] = {
      0,  9,  12, 9,  21, 12, // bottom 
      1,  10,  7,  7,  4,  1, // right
      2,  5,  14, 2,  14, 17, // front
      15,  6,  3, 15,  18, 6, // top
      8,  11, 20, 11, 20, 23, // back
      13, 16, 19, 13, 19, 22, // left
  };

  memcpy(droneModel->indices, indices, 6 * 6 * 4);

  /* Rect indices
  droneModel->indices[0] = 0;
  droneModel->indices[1] = 1;
  droneModel->indices[2] = 2;
  droneModel->indices[3] = 3;

  droneModel->indices[4] = 4;
  droneModel->indices[5] = 5;
  droneModel->indices[6] = 6;
  droneModel->indices[7] = 7;

  droneModel->indices[8] = 4;
  droneModel->indices[9] = 0;
  droneModel->indices[10] = 3;
  droneModel->indices[11] = 7;

  droneModel->indices[12] = 5;
  droneModel->indices[13] = 1;
  droneModel->indices[14] = 2;
  droneModel->indices[15] = 6;

  droneModel->indices[16] = 4;
  droneModel->indices[17] = 0;
  droneModel->indices[18] = 1;
  droneModel->indices[19] = 5;

  droneModel->indices[20] = 7;
  droneModel->indices[21] = 3;
  droneModel->indices[22] = 2;
  droneModel->indices[23] = 6;
  */

  /* Lines
  droneModel->indices[0] = 0;
  droneModel->indices[1] = 1;
  droneModel->indices[2] = 1;
  droneModel->indices[3] = 2;
  droneModel->indices[4] = 2;
  droneModel->indices[5] = 3;
  droneModel->indices[6] = 3;
  droneModel->indices[7] = 0;
  droneModel->indices[8] = 0;
  droneModel->indices[9] = 4;
  droneModel->indices[10] = 4;
  droneModel->indices[11] = 5;
  droneModel->indices[12] = 5;
  droneModel->indices[13] = 6;
  droneModel->indices[14] = 6;
  droneModel->indices[15] = 7;
  droneModel->indices[16] = 7;
  droneModel->indices[17] = 4;
  droneModel->indices[18] = 5;
  droneModel->indices[19] = 1;
  droneModel->indices[20] = 6;
  droneModel->indices[21] = 2;
  droneModel->indices[22] = 7;
  droneModel->indices[23] = 3;
  droneModel->indices[24] = 8;
  droneModel->indices[25] = 9;
  droneModel->indices[26] = 9;
  droneModel->indices[27] = 10;
  droneModel->indices[28] = 10;
  droneModel->indices[29] = 11;
  droneModel->indices[30] = 11;
  droneModel->indices[31] = 8;
  droneModel->indices[32] = 8;
  droneModel->indices[33] = 12;
  droneModel->indices[34] = 12;
  droneModel->indices[35] = 13;
  droneModel->indices[36] = 13;
  droneModel->indices[37] = 14;
  droneModel->indices[38] = 14;
  droneModel->indices[39] = 15;
  droneModel->indices[40] = 15;
  droneModel->indices[41] = 12;
  droneModel->indices[42] = 13;
  droneModel->indices[43] = 9;
  droneModel->indices[44] = 14;
  droneModel->indices[45] = 10;
  droneModel->indices[46] = 15;
  droneModel->indices[47] = 11;
  */

  return droneModel;
}

void drone_model_destroy(Model *droneModel) { free(droneModel); }

void drone_model_draw(GUI *gui, const GUIData *data) {
  Model *droneModel = gui->droneModel;

  mat4 projection;
  // mat4 projection, viewPort;
  // create_view_port(gui->width / 2.0f, gui->height, 100.0f, 0.1f, viewPort);
  create_mvp((float)gui->width / gui->height, deg_to_rad(60.0f), 100.0f,
             0.1f, projection);

  GLuint uOrientation =
      glGetUniformLocation(gui->shaderProgram, "uOrientation");
  GLuint uProjection = glGetUniformLocation(gui->shaderProgram, "uProjection");

  glUniform4f(uOrientation, data->sensorState->orientation.w,
              data->sensorState->orientation.i,
              data->sensorState->orientation.j,
              data->sensorState->orientation.k);

  glUniformMatrix4fv(uProjection, 1, GL_FALSE, (float *)projection);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glUseProgram(gui->shaderProgram);
  glBindVertexArray(gui->VAO);
  glDrawElements(GL_TRIANGLES, sizeof(gui->droneModel->indices)/sizeof(int), GL_UNSIGNED_INT, 0);
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

void text_draw(SDL_Renderer *renderer, int x, int y, const char *text,
               int fontSize, const SDL_Color *color) {
  TTF_Font *font = TTF_OpenFont("../fonts/Montserrat-Regular.ttf", fontSize);
  if (font == NULL) {
    printf("Error opening font : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Surface *surface = TTF_RenderText_Solid(font, text, *color);

  SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, surface);

  TTF_CloseFont(font);

  SDL_FRect textRect;
  textRect.x = x;
  textRect.y = y;
  textRect.w = surface->w;
  textRect.h = surface->h;

  SDL_RenderTexture(renderer, textTex, NULL, &textRect);
  SDL_DestroySurface(surface);
  SDL_DestroyTexture(textTex);
}
