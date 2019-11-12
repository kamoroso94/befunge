#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include "list.h"
#include "befunge.h"
#include "befunge_ops.h"

#define MAX(A, B) ((A) >= (B) ? (A) : (B))

// TODO: add unique error codes

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
  state->string_mode = false;
  state->is_running = true;

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

int bef_state_validate(bef_state_t *state) {
  list_t source_row;

  if (state == NULL || !state->is_running) return 1;
  if (state->source_grid.elsize != sizeof(list_t)) return 1;
  if (state->source_grid.length == 0) return 1;
  // TODO: check list exists?
  if (state->stack.list.elsize != sizeof(int)) return 1;
  return 0;
}

/** Runs the Befunge program.  Returns 0 iff successful. */
int bef_run(bef_state_t *state, unsigned seed) {
  if (bef_state_validate(state)) return 1;
  srand(seed);

  while (state->is_running) {
    if (bef_step(state)) return 1;
  }
  return 0;
}

int bef_step(bef_state_t *state) {
  int command;
  if (bef_state_validate(state)) return 1;
  if (bef_command_get(state, state->ip_row, state->ip_col, &command) return 1;
  if (bef_exec_op(state, command)) return 1;
  return bef_move(state);
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

/**
 * Gets the command at source_grid[row, col] or ' ' if it doesn't exist.
 * Returns 0 iff successful.
 */
int bef_source_get(
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

/**
 * Sets source_grid[row, col] to command.  Fills in new elements with ' '.
 * Returns 0 iff successful.
 */
int bef_source_set(
    bef_state_t *state, size_t row, size_t col, int command) {
  list_t source_row;
  size_t i;
  int filler = ' ';

  if (state == NULL || command == NULL) return 1;
  if (row < state->source_grid.length) {
    if (list_get(&state->source_grid, row, &source_row)) return 1;
  } else {
    if (list_init(&source_row, sizeof(int))) return 1;
    if (list_set(&state->source_grid, row, &source_row)) return 1;
  }

  for (i = source_row.length; i < col; i++) {
    if (list_set(&source_row, col, &filler)) return 1;
  }
  if (list_set(&source_row, col, &command)) return 1;
  return list_set(&state->source_grid, row, &source_row);
  // TODO: put back list after mutation
}

/** Push from value onto stack.  Returns 0 iff successful. */
static int bef_stack_push(bef_state_t *state, int value) {
  if (state == NULL) return 1;
  return stack_push(&state->stack, &value);
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

  if (bef_state_validate(state)) return 1;
  do {
    if (list_get(&state->source_grid, state->ip_row, &source_row)) return 1;
    row = state->ip_row;
    col = state->ip_col;
    width = MAX(1, state->source_grid.length);
    height = MAX(1, source_row.length);

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
