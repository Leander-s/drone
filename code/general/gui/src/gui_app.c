#include <colors.h>
#include <gui_app.h>
#include <gui_config.h>

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

void createBuffer(GLuint *vao, GLuint *vbo, GLuint *ebo, int position,
                  int normal, int color, int texture) {
        glGenVertexArrays(1, vao);
        glBindVertexArray(*vao);

        if (vbo != NULL) {
                glGenBuffers(1, vbo);
        }
        if (ebo != NULL) {
                glGenBuffers(1, ebo);
        }

        glBindBuffer(GL_ARRAY_BUFFER, *vbo);

        int vertexSize = position + color + normal + texture;
        int offset = 0;

        if (position != 0) {
                // position attribute
                glVertexAttribPointer(offset, position, GL_FLOAT, GL_FALSE,
                                      vertexSize * sizeof(float), (void *)0);
                glEnableVertexAttribArray(offset);
                offset++;
        }

        if (normal != 0) {
                // normal attribute
                glVertexAttribPointer(offset, normal, GL_FLOAT, GL_FALSE,
                                      vertexSize * sizeof(float),
                                      (void *)(position * sizeof(float)));
                glEnableVertexAttribArray(offset);
                offset++;
        }

        if (color != 0) {
                // color attribute
                glVertexAttribPointer(
                    offset, color, GL_FLOAT, GL_FALSE,
                    vertexSize * sizeof(float),
                    (void *)((position + normal) * sizeof(float)));
                glEnableVertexAttribArray(offset);
                offset++;
        }

        if (texture != 0) {
                // color attribute
                glVertexAttribPointer(
                    offset, texture, GL_FLOAT, GL_FALSE,
                    vertexSize * sizeof(float),
                    (void *)((position + normal + color) * sizeof(float)));
                glEnableVertexAttribArray(offset);
                offset++;
        }

        glBindVertexArray(0);
}

void sendToBuffer(GLuint vao, GLuint vertexBuffer, GLuint indexBuffer,
                  const float *vertices, int vertexCount, int vertexSize,
                  const int *indices, int indexCount) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize * sizeof(float),
                     vertices, GL_STATIC_DRAW);

        if (indices == NULL) {
                glBindVertexArray(0);
                return;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indices,
                     GL_STATIC_DRAW);
        glBindVertexArray(0);
}

void setupBuffers(GUI *gui) {
        createBuffer(&gui->VAO_3D, &gui->VBO_3D, &gui->IBO_3D, 3, 3, 3, 0);
        createBuffer(&gui->VAO_HUD_py_indicator, &gui->VBO_HUD_py_indicator,
                     &gui->IBO_HUD_py_indicator, 2, 0, 3, 0);
        createBuffer(&gui->VAO_HUD_roll, &gui->VBO_HUD_roll, &gui->IBO_HUD_roll,
                     2, 0, 3, 0);
        createBuffer(&gui->VAO_HUD_lines, &gui->VBO_HUD_lines,
                     &gui->IBO_HUD_lines, 2, 0, 3, 0);
        createBuffer(&gui->VAO_HUD_throttle, &gui->VBO_HUD_throttle,
                     &gui->IBO_HUD_throttle, 2, 0, 3, 0);
        createBuffer(&gui->VAO_text, &gui->VBO_text, NULL, 2, 0, 0, 2);

        // 3D
        sendToBuffer(gui->VAO_3D, gui->VBO_3D, gui->IBO_3D,
                     gui->droneModel->vertices, gui->droneModel->vertexCount,
                     gui->droneModel->vertexSize, gui->droneModel->indices,
                     gui->droneModel->indexCount);

        // text
        float vertices[] = {
            // x, y,   u, v
            0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
            1.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, // Top-right

            0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
            1.0f, 1.0f, 1.0f, 0.0f, // Top-right
            0.0f, 1.0f, 0.0f, 0.0f  // Top-left
        };

        sendToBuffer(gui->VAO_text, gui->VBO_text, gui->IBO_text, vertices, 6,
                     4, NULL, 0);

        // HUD
        // HUD lines
        sendToBuffer(gui->VAO_HUD_lines, gui->VBO_HUD_lines, gui->IBO_HUD_lines,
                     gui->hudModels[0]->vertices,
                     gui->hudModels[0]->vertexCount,
                     gui->hudModels[0]->vertexSize, gui->hudModels[0]->indices,
                     gui->hudModels[0]->indexCount);
        // HUD py_indicator
        sendToBuffer(gui->VAO_HUD_py_indicator, gui->VBO_HUD_py_indicator,
                     gui->IBO_HUD_py_indicator, gui->hudModels[1]->vertices,
                     gui->hudModels[1]->vertexCount,
                     gui->hudModels[1]->vertexSize, gui->hudModels[1]->indices,
                     gui->hudModels[1]->indexCount);
        // HUD roll
        sendToBuffer(gui->VAO_HUD_roll, gui->VBO_HUD_roll, gui->IBO_HUD_roll,
                     gui->hudModels[2]->vertices,
                     gui->hudModels[2]->vertexCount,
                     gui->hudModels[2]->vertexSize, gui->hudModels[2]->indices,
                     gui->hudModels[2]->indexCount);
        // HUD throttle
        sendToBuffer(gui->VAO_HUD_throttle, gui->VBO_HUD_throttle,
                     gui->IBO_HUD_throttle, gui->hudModels[3]->vertices,
                     gui->hudModels[3]->vertexCount,
                     gui->hudModels[3]->vertexSize, gui->hudModels[3]->indices,
                     gui->hudModels[3]->indexCount);
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
        int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result < 0) {
                SDL_Log("SDL_Init failed : %s\n", SDL_GetError());
                return NULL;
        }

        if (!TTF_Init()) {
                printf("Error initializing ttf. Text cannot be rendered.\n");
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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        gui->window =
            SDL_CreateWindow("Drone Controller", width, height,
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
        if (!SDL_GL_MakeCurrent(gui->window, gui->glContext)) {
                fprintf(stderr, "SDL_GL_MakeCurrent Error: %s\n",
                        SDL_GetError());
                SDL_GL_DestroyContext(gui->glContext);
                SDL_DestroyWindow(gui->window);
                SDL_Quit();
                return NULL;
        }

        printf("glContext created\n");

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
                SDL_Log("gladLoadGLLoader failed");
                SDL_GL_DestroyContext(gui->glContext);
                SDL_DestroyWindow(gui->window);
                SDL_Quit();
                return NULL;
        }

        const GLubyte *version = glGetString(GL_VERSION);
        if (!version) {
                fprintf(stderr, "No valid OpenGL context!\n");
                SDL_GL_DestroyContext(gui->glContext);
                SDL_DestroyWindow(gui->window);
                SDL_Quit();
                return NULL;
        }

        printf("OpenGL version: %s\n", version);

        gui->droneModel = drone_model_create();
        gui->hudModels = hud_models_create();

        glEnable(GL_DEPTH_TEST);
        int err = glGetError();
        if (err != GL_NO_ERROR) {
                printf("Error in depth test: 0x%x\n", err);
        }

        gui->shaderProgram_3D =
            setupShaders("../shaders/shader.vert", "../shaders/shader.frag");
        gui->shaderProgram_text =
            setupShaders("../shaders/text.vert", "../shaders/text.frag");
        gui->shaderProgram_HUD_roll =
            setupShaders("../shaders/hud_roll.vert", "../shaders/hud.frag");
        gui->shaderProgram_HUD_lines =
            setupShaders("../shaders/hud_lines.vert", "../shaders/hud.frag");
        gui->shaderProgram_HUD_throttle =
            setupShaders("../shaders/hud_throttle.vert", "../shaders/hud.frag");
        gui->shaderProgram_HUD_rects =
            setupShaders("../shaders/hud_rect.vert", "../shaders/hud.frag");

        setupBuffers(gui);

        gui->updateCounter = 0;
        memset(gui->keyState, 0, 322);

        return gui;
}

void gui_update(GUI *gui, const GUIData *data) {
        glClearColor(0.021f, 0.043f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, gui->width / 2, gui->height);
        hud_draw(gui, data);
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
                SDL_GetWindowSizeInPixels(gui->window, &gui->width,
                                          &gui->height);
                break;
        }

        gui->updateCounter = (gui->updateCounter + 1) % 1;
}

void gui_destroy(GUI *gui) {
        glDeleteBuffers(1, &gui->VBO_3D);
        glDeleteBuffers(1, &gui->VAO_3D);
        glDeleteBuffers(1, &gui->IBO_3D);
        glDeleteBuffers(1, &gui->VAO_HUD_throttle);
        glDeleteBuffers(1, &gui->VBO_HUD_throttle);
        glDeleteBuffers(1, &gui->IBO_HUD_throttle);
        glDeleteBuffers(1, &gui->VAO_HUD_lines);
        glDeleteBuffers(1, &gui->VBO_HUD_lines);
        glDeleteBuffers(1, &gui->IBO_HUD_lines);
        glDeleteBuffers(1, &gui->VAO_HUD_py_indicator);
        glDeleteBuffers(1, &gui->VBO_HUD_py_indicator);
        glDeleteBuffers(1, &gui->IBO_HUD_py_indicator);
        glDeleteBuffers(1, &gui->VAO_HUD_roll);
        glDeleteBuffers(1, &gui->VBO_HUD_roll);
        glDeleteBuffers(1, &gui->IBO_HUD_roll);
        glDeleteBuffers(1, &gui->VBO_text);
        glDeleteBuffers(1, &gui->VAO_text);
        glDeleteBuffers(1, &gui->IBO_text);
        model_destroy(gui->droneModel);
        for (int i = 0; i < 4; i++) {
                model_destroy(gui->hudModels[i]);
        }
        SDL_GL_DestroyContext(gui->glContext);
        SDL_DestroyWindow(gui->window);
        SDL_Quit();
}

void hud_draw(GUI *gui, const GUIData *data) {
        glUseProgram(gui->shaderProgram_HUD_lines);
        glBindVertexArray(gui->VAO_HUD_lines);
        glDrawElements(GL_LINES, gui->hudModels[0]->indexCount, GL_UNSIGNED_INT,
                       0);

        glUseProgram(gui->shaderProgram_HUD_rects);
        vec2 translation = {-0.7f, -0.5f};
        translation[0] +=
            ((float)(data->controlState->yaw - 127) / 255 * 2.0f) * 0.1f;
        translation[1] +=
            ((float)(data->controlState->pitch - 127) / 255 * 2.0f) * 0.1f;
        glBindVertexArray(gui->VAO_HUD_py_indicator);
        GLuint uTranslation =
            glGetUniformLocation(gui->shaderProgram_HUD_rects, "uTranslation");
        glUniform2f(uTranslation, translation[0], translation[1]);
        glDrawElements(GL_TRIANGLES, gui->hudModels[1]->indexCount,
                       GL_UNSIGNED_INT, 0);

        glUseProgram(gui->shaderProgram_HUD_roll);
        glBindVertexArray(gui->VAO_HUD_roll);
        float roll = (float)data->controlState->roll / 255 * 2.0f - 1.0f;
        GLuint uAngle =
            glGetUniformLocation(gui->shaderProgram_HUD_roll, "uAngle");
        glUniform1f(uAngle, roll);
        glDrawElements(GL_LINES, gui->hudModels[2]->indexCount, GL_UNSIGNED_INT,
                       0);

        glUseProgram(gui->shaderProgram_HUD_throttle);
        glBindVertexArray(gui->VAO_HUD_throttle);
        float throttle = (float)data->controlState->throttle / 255;
        GLuint uThrottle =
            glGetUniformLocation(gui->shaderProgram_HUD_throttle, "uThrottle");
        glUniform1f(uThrottle, throttle);
        glDrawElements(GL_TRIANGLES, gui->hudModels[3]->indexCount,
                       GL_UNSIGNED_INT, 0);
}

void data_sheet_draw(GUI *gui, const GUIData *data) {
        const int x = gui->width / 2;
        const int y = 0;
        const int width = gui->width - x;
        const int height = gui->height;

        const int padding = 20;
        const int textX = x + padding;
        int yOffset = padding;

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
        sprintf(usbReadErrorsBuffer, "USB read errors: %d",
                data->log->usbReadErrors);
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

Model **hud_models_create() {
        Model **hud = malloc(4 * sizeof(Model *));

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
            0.8f, -0.8f, 1.0f, 1.0f, 1.0f, // bottom-right     8
            0.7f, -0.8f, 1.0f, 1.0f, 1.0f, // bottom-left      9
            0.7f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-left         10
            0.8f, 0.0f, 1.0f, 1.0f, 1.0f,  // bottom-left      11
        };

        int lines_indices[] = {                        // Pitch/Yaw lines
                               0, 1, 1, 2, 2, 3, 3, 0, // outline
                               4, 5, 6, 7,             // cross

                               // Throttle lines
                               8, 9, 9, 10, 10, 11, 11, 8};

        hud[0] = malloc(sizeof(Model));
        hud[0]->vertexCount = sizeof(lines_verts) / sizeof(float);
        hud[0]->vertices = malloc(sizeof(lines_verts));
        memcpy(hud[0]->vertices, lines_verts, sizeof(lines_verts));

        hud[0]->indexCount = sizeof(lines_indices) / sizeof(float);
        hud[0]->indices = malloc(sizeof(lines_indices));
        memcpy(hud[0]->indices, lines_indices, sizeof(lines_indices));

        hud[0]->vertexSize = 5;

        // indicator for pitch/yaw
        float py_ind_verts[] = {
            -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, // bottom-left      0
            0.01f,  -0.01f, 1.0f, 1.0f, 1.0f, // bottom-right     1
            0.01f,  0.01f,  1.0f, 1.0f, 1.0f, // top-right        2
            -0.01f, 0.01f,  1.0f, 1.0f, 1.0f, // top-left         3
        };

        int py_ind_indices[] = {0, 1, 2, 2, 3, 0};

        hud[1] = malloc(sizeof(Model));
        hud[1]->vertexCount = sizeof(py_ind_verts) / sizeof(float);
        hud[1]->vertices = malloc(sizeof(py_ind_verts));
        memcpy(hud[1]->vertices, py_ind_verts, sizeof(py_ind_verts));

        hud[1]->indexCount = sizeof(py_ind_indices) / sizeof(float);
        hud[1]->indices = malloc(sizeof(py_ind_indices));
        memcpy(hud[1]->indices, py_ind_indices, sizeof(py_ind_indices));

        hud[1]->vertexSize = 5;

        // lines for roll
        float roll_verts[] = {
            -0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f,
        };

        int roll_indices[] = {0, 1};

        hud[2] = malloc(sizeof(Model));
        hud[2]->vertexCount = sizeof(roll_verts) / sizeof(float);
        hud[2]->vertices = malloc(sizeof(roll_verts));
        memcpy(hud[2]->vertices, roll_verts, sizeof(roll_verts));

        hud[2]->indexCount = sizeof(roll_indices) / sizeof(float);
        hud[2]->indices = malloc(sizeof(roll_indices));
        memcpy(hud[2]->indices, roll_indices, sizeof(roll_indices));

        hud[2]->vertexSize = 5;

        // indicator for throttle
        float throttle_verts[] = {
            -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 0.05f,  0.0f, 1.0f, 1.0f, 1.0f,
            0.05f,  1.0f, 1.0f, 1.0f, 1.0f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f,
        };

        int throttle_indices[] = {0, 1, 2, 2, 3, 0};

        hud[3] = malloc(sizeof(Model));
        hud[3]->vertexCount = sizeof(throttle_verts) / sizeof(float);
        hud[3]->vertices = malloc(sizeof(throttle_verts));
        memcpy(hud[3]->vertices, throttle_verts, sizeof(throttle_verts));

        hud[3]->indexCount = sizeof(throttle_indices) / sizeof(float);
        hud[3]->indices = malloc(sizeof(throttle_indices));
        memcpy(hud[3]->indices, throttle_indices, sizeof(throttle_indices));

        hud[3]->vertexSize = 5;

        return hud;
}

Model *drone_model_create() {
        Model *droneModel = malloc(sizeof(Model));
        float verts[] = {
            // right, bottom, front
            0.3f, -0.1f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            1.0f,                                                   // 0, bottom
            0.3f, -0.1f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 1, right
            0.3f, -0.1f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 2, front

            // right, top, front
            0.3f, 0.1f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 3, top
            0.3f, 0.1f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 4, right
            0.3f, 0.1f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 5, front

            // right, top, back
            0.3f, 0.1f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 6, top
            0.3f, 0.1f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 7, right
            0.3f, 0.1f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 8, back

            // right, bottom, back
            0.3f, -0.1f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 9, bottom
            0.3f, -0.1f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 10, right
            0.3f, -0.1f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // 11, back

            // left, bottom, front
            -0.3f, -0.1f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, // 12, bottom
            -0.3f, -0.1f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, // 13, left
            -0.3f, -0.1f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.0f, // 14, front

            // left, top, front
            -0.3f, 0.1f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 15, top
            -0.3f, 0.1f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 16, left
            -0.3f, 0.1f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.0f, // 17, front

            // left, top, back
            -0.3f, 0.1f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // 18, top
            -0.3f, 0.1f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 19, left
            -0.3f, 0.1f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // 20, back

            // left, bottom, back
            -0.3f, -0.1f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, // 21, bottom
            -0.3f, -0.1f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 22, left
            -0.3f, -0.1f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // 23, back
        };

        droneModel->vertexCount = sizeof(verts) / 4;
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

        droneModel->indexCount = sizeof(indices) / 4;
        droneModel->indices = malloc(sizeof(indices));
        memcpy(droneModel->indices, indices, sizeof(indices));

        droneModel->vertexSize = 6;

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
        // create_view_port(gui->width / 2.0f, gui->height, 100.0f, 0.1f,
        // viewPort);
        create_mvp((float)gui->width / 2 / gui->height, deg_to_rad(60.0f),
                   100.0f, 0.1f, projection);
        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model, (vec3){0.0f, 0.0f, -1.0f});
        glm_scale(model, (vec3){1.0f, 1.0f, 1.0f});
        glm_mat4_mul(projection, model, projection);

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

void text_draw(GUI *gui, int x, int y, const char *text, int fontSize,
               const SDL_Color *color) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        TTF_Font *font =
            TTF_OpenFont("../fonts/Montserrat-Regular.ttf", fontSize);
        if (font == NULL) {
                printf("Error opening font.\n");
                exit(EXIT_FAILURE);
        }

        SDL_Surface *incorrect_surface =
            TTF_RenderText_Blended(font, text, strlen(text), *color);
        SDL_Surface *surface =
            SDL_ConvertSurface(incorrect_surface, SDL_PIXELFORMAT_ABGR8888);
        TTF_CloseFont(font);
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

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

        GLuint tex_loc =
            glGetUniformLocation(gui->shaderProgram_text, "uTexture");
        glUniform1i(tex_loc, 0);

        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model,
                      (vec3){(float)x, (float)(gui->height - 2 * y), 0.0f});
        glm_scale(model, (vec3){(float)tex_w, (float)tex_h, 1.0f});

        mat4 proj_model;
        GLint proj_loc =
            glGetUniformLocation(gui->shaderProgram_text, "uProjection");
        glm_mat4_mul(projection, model, proj_model);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (float *)proj_model);

        glBindVertexArray(gui->VAO_text);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glDeleteTextures(1, &texture);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
}
