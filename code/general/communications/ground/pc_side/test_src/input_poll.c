#include <input_poll.h>

#ifdef _WIN32
int key_in_stack(inputHandler *handler, int keycode) {
  for (int i = 0; i < handler->stack_offset; i++) {
    if (handler->key_stack[i] == keycode)
      return 1;
    if (handler->key_stack[i] == 0)
      return 0;
  }
  return 0;
}

void remove_from_stack(inputHandler *handler, int keycode) {
  int removed = 0;
  unsigned char next_key;
  for (int i = 0; i < handler->stack_offset; i++) {
    int next_index = i + 1;
    if (next_index == handler->stack_offset) {
      next_key = 0;
    } else {
      next_key = handler->key_stack[next_index];
    }
    if (handler->key_stack[i] == keycode || removed) {
      handler->key_stack[i] = next_key;
      removed = 1;
    }
    if (next_key == 0) {
      return;
    }
  }
}

void prepend_to_stack(inputHandler *handler, int keycode) {
  handler->stack_offset += 1;
  for (int i = 0; i < handler->stack_offset; i++) {
    if (handler->key_stack[i] == keycode) {
      handler->key_stack[i] = 0;
      handler->stack_offset -= 1;
    }
  }

  int stored_key = keycode;
  for (int i = 0; i < handler->stack_offset + 1; i++) {
    if (stored_key == 0) {
      break;
    }
    int temp = handler->key_stack[i];
    handler->key_stack[i] = stored_key;
    stored_key = temp;
  }
}

inputHandler *createInputHandler() {
  inputHandler *result = malloc(sizeof(inputHandler));
  result->key_stack_size = 256;
  result->key_stack = malloc(sizeof(unsigned char) * result->key_stack_size);
  memset(result->key_stack, 0, result->key_stack_size);
  result->stack_offset = 0;

  return result;
}

void destroyInputHandler(inputHandler *handler) {
  free(handler->key_stack);
  free(handler);
}

int key_down(inputHandler *handler, int key) {
    int keys[256];
    int result = GetKeyState(key) & 0x8000;
    if (result == 0) {
        remove_from_stack(handler, key);
        return result;
    }

    prepend_to_stack(handler, key);

    return result;
}

int key_pressed(inputHandler *handler, int key) {
    if (key_in_stack(handler, key)) {
        key_down(handler, key);
        return 0;
    }

    return key_down(handler, key);
}

#else

int key_in_stack(inputHandler *handler, KeySym key) {
  KeyCode keycode = XKeysymToKeycode(handler->display, key);
  for (int i = 0; i < handler->stack_offset; i++) {
    if (handler->key_stack[i] == keycode)
      return 1;
    if (handler->key_stack[i] == 0)
      return 0;
  }
  return 0;
}

void remove_from_stack(inputHandler *handler, KeySym key) {
  KeyCode keycode = XKeysymToKeycode(handler->display, key);
  int removed = 0;
  unsigned char next_key;
  for (int i = 0; i < handler->stack_offset; i++) {
    int next_index = i + 1;
    if (next_index == handler->stack_offset) {
      next_key = 0;
    } else {
      next_key = handler->key_stack[next_index];
    }
    if (handler->key_stack[i] == keycode || removed) {
      handler->key_stack[i] = next_key;
      removed = 1;
    }
    if (next_key == 0) {
      return;
    }
  }
}

void prepend_to_stack(inputHandler *handler, KeySym key) {
  KeyCode keycode = XKeysymToKeycode(handler->display, key);
  handler->stack_offset += 1;
  for (int i = 0; i < handler->stack_offset; i++) {
    if (handler->key_stack[i] == keycode) {
      handler->key_stack[i] = 0;
      handler->stack_offset -= 1;
    }
  }

  unsigned char stored_key = keycode;
  for (int i = 0; i < handler->stack_offset + 1; i++) {
    if (stored_key == 0) {
      break;
    }
    unsigned char temp = handler->key_stack[i];
    handler->key_stack[i] = stored_key;
    stored_key = temp;
  }
}

inputHandler *createInputHandler() {
  inputHandler *result = malloc(sizeof(inputHandler));
  result->key_stack_size = 256;
  result->key_stack = malloc(sizeof(unsigned char) * result->key_stack_size);
  memset(result->key_stack, 0, result->key_stack_size);
  result->stack_offset = 0;
  result->display = XOpenDisplay(NULL);
  if (result->display == NULL) {
    perror("Display was null\n");
  }

  disable_terminal_input(&result->original_terminal_settings);

  return result;
}

void destroyInputHandler(inputHandler *handler) {
  restore_terminal_input(&handler->original_terminal_settings);
  free(handler->key_stack);
  free(handler);
}

int key_down(inputHandler *handler, KeySym key) {
    char keys[256];
    KeyCode keycode = XKeysymToKeycode(handler->display, key);
    XQueryKeymap(handler->display, keys);
    int result = keys[keycode / 8] & (1 << (keycode % 8));
    if (result == 0) {
        remove_from_stack(handler, key);
        return result;
    }

    prepend_to_stack(handler, key);

    return result;
}

int key_pressed(inputHandler *handler, KeySym key) {
    KeyCode keycode = XKeysymToKeycode(handler->display, key);
    if (key_in_stack(handler, key)) {
        key_down(handler, key);
        return 0;
    }

    return key_down(handler, key);
}
#endif
