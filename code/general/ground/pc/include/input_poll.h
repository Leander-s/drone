#include "util.h"

typedef struct {
  Display *display;
  int key_stack_size;
  unsigned char* key_stack;
  int stack_offset;
  struct termios original_terminal_settings;
} inputHandler;

inputHandler *createInputHandler();

int key_down(inputHandler *handler, KeySym key);
int key_pressed(inputHandler *handler, KeySym key);

void destroyInputHandler(inputHandler *handler);
