#pragma once
#include "util.h"

#ifdef _WIN32
typedef struct {
  int key_stack_size;
  int* key_stack;
  int stack_offset;
} inputHandler;

int key_down(inputHandler *handler, int key);
int key_pressed(inputHandler *handler, int key);
#else
typedef struct {
  Display *display;
  int key_stack_size;
  unsigned char* key_stack;
  int stack_offset;
  struct termios original_terminal_settings;
} inputHandler;

int key_down(inputHandler *handler, KeySym key);
int key_pressed(inputHandler *handler, KeySym key);
#endif

inputHandler *createInputHandler();


void destroyInputHandler(inputHandler *handler);
