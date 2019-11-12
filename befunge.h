#ifndef BEFUNGE_H
#define BEFUNGE_H

#include <stddef.h>
#include <stdbool.h>
#include "list.h"
#include "stack.h"

typedef enum {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_COUNT} dir_t;

typedef struct {
  list_t source_grid;
  stack_t stack;
  dir_t ip_dir;
  size_t ip_row, ip_col;
  bool string_mode, is_running;
} bef_state_t;

int bef_init(bef_state_t *state, const char *source, size_t len);
int bef_state_validate(bef_state_t *state);
int bef_run(bef_state_t *state);
int bef_source_get(bef_state_t *state, size_t row, size_t col, int *value);
int bef_source_set(bef_state_t *state, size_t row, size_t col, int value);
int bef_stack_push(bef_state_t *state, int value);
int bef_stack_pop(bef_state_t *state, int *value);
int bef_stack_peek(bef_state_t *state, int *value);
int bef_move(bef_state_t *state);
void bef_destroy(bef_state_t *state);

#endif /* BEFUNGE_H */
