#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include "list.h"
#include "befunge.h"

static int bef_validate(bef_state_t *state);

// TODO: Move ops to separate file
// Stretch: add unique error codes

/** Initializes the Befunge state object.  Returns 0 iff successful. */
int bef_init(bef_state_t *state, const char *source, size_t len) {
  size_t row, col, i;
  int c;

  if (state == NULL || source == NULL || len == 0) return 1;
  if (list_init(&state->source_grid, sizeof(list_t))) return 1;
  if (stack_init(&state->stack, sizeof(int))) return 1;
  state->ip_row = 0;
  state->ip_col = 0;
  state->ip_dir = DIR_RIGHT;
  sate->string_mode = false;

  row = 0;
  col = 0;
  for (i = 0; i < len; i++) {
    c = source[i];
    if (c == '\n') {
      row++;
      col = 0;
      continue;
    }

    if (bef_source_set(state, row, col, &c)) return 1;
    col++;
  }

  return 0;
}

/** Runs the Befunge program.  Returns 0 iff successful. */
int bef_run(bef_state_t *state, unsigned seed) {
  list_t source_row;
  int command, value1, value2, value3, result, sign;

  if (bef_validate(state)) return 1;
  srand(seed);

  while (true) {
    if (bef_command_get(state, state->ip_row, state->ip_col, &command) return 1;

    if (state->string_mode && command != '"') {
      if (bef_stack_push(state, &command)) return 1;
      if (bef_move(state)) return 1;
      continue;
    }

    switch (c) {
      case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9': {
        result = c - '0';
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': {
        result = c - 'a';
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '+': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        result = value1 + value2;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '-': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        result = value2 - value1;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '*': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        result = value1 * value2;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '/': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;

        sign = value1 < 0 != value2 < 0 ? -1 : 1;
        if (value1 < 0) value1 *= -1;
        if (value2 < 0) value2 *= -1;
        result = sign * (value2 / value1);
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '%': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;

        if (value1 < 0) value1 *= -1;
        if (value2 < 0) value2 *= -1;
        result = value2 % value1;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '!': {
        if (bef_stack_pop(state, &value1)) return 1;
        result = !value1;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '`': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        result = value2 > value1;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '>': state->ip_dir = DIR_RIGHT; break;
      case '<': state->ip_dir = DIR_LEFT; break;
      case '^': state->ip_dir = DIR_UP; break;
      case 'v': state->ip_dir = DIR_DOWN; break;
      case '?': {
        state->ip_dir = (dir_t) ((double) rand() / RAND_MAX * DIR_COUNT);
        break;
      }
      case '_': {
        if (bef_stack_pop(state, &value1)) return 1;
        state->ip_dir = value1 ? DIR_LEFT : DIR_RIGHT;
        break;
      }
      case '|': {
        if (bef_stack_pop(state, &value1)) return 1;
        state->ip_dir = value1 ? DIR_UP : DIR_DOWN;
        break;
      }
      case '"': state->string_mode = !state->string_mode; break;
      case ':': {
        if (bef_stack_peek(state, &result)) return 1;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '\\': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        if (bef_stack_push(state, &value1)) return 1;
        if (bef_stack_push(state, &value2)) return 1;
        break;
      }
      case '$': bef_stack_pop(state, &value1); break;
      case '.': if (printf("%d ", c) < 0) return 1; break;
      case ',': if (putchar(c) == EOF) return 1; break;
      case '#': if (bef_move(state)) return 1; break;
      case 'p': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        if (bef_stack_pop(state, &value3)) return 1;
        if (bef_source_set(state, value1, value2, &value3)) return 1;
        break;
      }
      case 'g': {
        if (bef_stack_pop(state, &value1)) return 1;
        if (bef_stack_pop(state, &value2)) return 1;
        if (bef_source_get(state, value1, value2, &result)) return 1;
        if (bef_stack_push(state, &result)) return 1;
        break;
      }
      case '&': {
        errno = 0;
        result = scanf("%d", &value1);
        if (errno) return 1;
        if (result != 1) value1 = 0;
        if (bef_stack_push(state, &value1)) return 1;
        break;
      }
      case '~': {
        errno = 0;
        result = getchar();
        if (errno) return 1;
        value1 = result != EOF ? result : 0;
        if (bef_stack_push(state, &value1)) return 1;
        break;
      }
      case '@': return 0;
      // custom
      case 'E': {
        result = errno;
        if (bef_stack_push(state, &result)) return 1;
        errno = 0;
        break;
      }

      default: break;
    }
    if (bef_move(state)) return 1;
  }
  return 0;
}

/** Destroys each row in the befunge source grid. */
void bef_destroy(bef_state_t *state) {
  list_t source_row;
  size_t i;

  if (state == NULL) return;
  stack_destroy(&state->stack);

  for (i = 0; i < state->source_grid.length; i++) {
    if (list_get(&state->source_grid, i, &source_row)) continue;
    list_destroy(&source_row);
  }
  list_destroy(&state->source_grid);
}

/** Gets the command at source_grid[row, col].  Returns 0 iff successful. */
static int bef_source_get(
    bef_state_t *state, size_t row, size_t col, int *command) {
  list_t source_row;

  if (state == NULL || command == NULL) return 1;
  if (row < state->source_grid.length) {
    if (list_get(state->source_grid, row, &source_row)) return 1;
    if (col < source_row.length) {
      return list_get(&source_row, col, command);
    }
  }
  *command = ' ';
  return 0;
}

/** Sets source_grid[row, col] to command.  Returns 0 iff successful. */
static int bef_source_set(
    bef_state_t *state, size_t row, size_t col, int *command) {
  list_t source_row;

  if (state == NULL || command == NULL) return 1;
  if (row < state->source_grid.length) {
    if (list_get(&state->source_grid, row, &source_row)) return 1;
  } else {
    if (list_init(&source_row, sizeof(int))) return 1;
    if (list_set(&state->source_grid, row, &source_row)) return 1;
  }
  return list_set(&source_row, col, command);
}

/** Push from value onto stack.  Returns 0 iff successful. */
static int bef_stack_push(bef_state_t *state, int *value) {
  if (state == NULL) return 1;
  return stack_push(&state->stack, value);
}

/** Pop from stack or 0 into value.  Returns 0 iff successful. */
static int bef_stack_pop(bef_state_t *state, int *value) {
  if (state == NULL || value == NULL) return 1;
  if (state->stack.list.length > 0) return stack_pop(&state->stack, value);
  *value = 0;
  return 0;
}

/** Peek from stack or 0 into value.  Returns 0 iff successful. */
static int bef_stack_peek(bef_state_t *state, int *value) {
  if (state == NULL || value == NULL) return 1;
  if (state->stack.list.length > 0) return stack_peek(&state->stack, value);
  *value = 0;
  return 0;
}

/**
 * Performs a move in dir direction from position (x, y).  Returns 0
 * iff successful.
 */
int bef_move(bef_state_t *state) {
  list_t source_row;
  int command;
  size_t row, col, width, height;

  if (bef_validate(state)) return 1;
  do {
    if (list_get(&state->source_grid, state->ip_row, &source_row)) return 1;
    row = state->ip_row;
    col = state->ip_col;
    width = state->source_grid.length;
    height = source_row.length;

    switch (state->ip_dir) {
      case DIR_UP:
      state->ip_row = row == 0 ? height - 1 : row - 1;
      break;

      case DIR_RIGHT:
      state->ip_col = col == width - 1 ? 0 : col + 1;
      break;

      case DIR_DOWN:
      state->ip_row = row == height - 1 ? 0 : row + 1;
      break;

      case DIR_LEFT:
      state->ip_col = col == 0 ? width - 1 : col - 1;
      break;

      default: return 1;
    }
    if (bef_get_command(state, state->row, state->col, &command)) return 1;
  } while (command == ' ');
  return 0;
}

static int bef_validate(bef_state_t *state) {
  if (state == NULL) return 1;
  if (state->source_grid.elsize != sizeof(list_t)) return 1;
  if (state->source_grid.length == 0) return 1;
  if (state->stack.list.elsize != sizeof(int)) return 1;
  return 0;
}
