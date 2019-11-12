#include <ctype.h>
#include <limits.h>
#include "list.h"
#include "befunge.h"
#include "befunge_ops.h"

#define BEF_OP_TABLE_SIZE (1 << CHAR_BIT)

static int bef_op_xdigit(bef_state_t *state);
static int bef_op_add(bef_state_t *state);
static int bef_op_sub(bef_state_t *state);
static int bef_op_mul(bef_state_t *state);
static int bef_op_div(bef_state_t *state);
static int bef_op_mod(bef_state_t *state);
static int bef_op_not(bef_state_t *state);
static int bef_op_gt(bef_state_t *state);
static int bef_op_look_right(bef_state_t *state);
static int bef_op_look_left(bef_state_t *state);
static int bef_op_look_up(bef_state_t *state);
static int bef_op_look_down(bef_state_t *state);
static int bef_op_rand_branch(bef_state_t *state);
static int bef_op_horz_branch(bef_state_t *state);
static int bef_op_vert_branch(bef_state_t *state);
static int bef_op_dupe(bef_state_t *state);
static int bef_op_swap(bef_state_t *state);
static int bef_op_discard(bef_state_t *state);
static int bef_op_print_int(bef_state_t *state);
static int bef_op_print_char(bef_state_t *state);
static int bef_op_skip(bef_state_t *state);
static int bef_op_put(bef_state_t *state);
static int bef_op_get(bef_state_t *state);
static int bef_op_scan_int(bef_state_t *state);
static int bef_op_scan_char(bef_state_t *state);
static int bef_op_exit(bef_state_t *state);
static int bef_op_error(bef_state_t *state);

int bef_exec_op(state_t *state, int command) {
  static int (* const bef_op_table[BEF_OP_TABLE_SIZE])(state_t *) = {
    ['0'] = bef_op_xdigit,  // [0-9a-f]
    ['+'] = bef_op_add,
    ['-'] = bef_op_sub,
    ['*'] = bef_op_mul,
    ['/'] = bef_op_div,
    ['%'] = bef_op_mod,
    ['!'] = bef_op_not,
    ['`'] = bef_op_gt,
    ['>'] = bef_op_look_right,
    ['<'] = bef_op_look_left,
    ['^'] = bef_op_look_up,
    ['v'] = bef_op_look_down,
    ['?'] = bef_op_rand_branch,
    ['_'] = bef_op_horz_branch,
    ['|'] = bef_op_vert_branch,
    ['"'] = bef_op_string_toggle,
    [':'] = bef_op_dupe,
    ['\\'] = bef_op_swap,
    ['$'] = bef_op_discard,
    ['.'] = bef_op_print_int,
    [','] = bef_op_print_char,
    ['#'] = bef_op_skip,
    ['p'] = bef_op_put,
    ['g'] = bef_op_get,
    ['&'] = bef_op_scan_int,
    ['~'] = bef_op_scan_char,
    ['@'] = bef_op_exit,
    ['E'] = bef_op_error,   // custom
  };

  if (bef_state_validate(state)) return 1;

  // <STRING>[^"]
  if (state->string_mode && command != '"') {
    return bef_stack_push(state, &command);
  }

  // [0-9a-f]
  if (isxdigit(command) && !isupper(command)) command = '0';

  if (command < 0 || command >= BEF_OP_TABLE_SIZE) return 0;
  return bef_op != NULL ? (*bef_op_table[command])(state) : 0;
}

static int bef_op_xdigit(bef_state_t *state) {
  int command, digit;
  if (bef_source_get(state, state->ip_row, state->ip_col, &command)) return 1;
  digit = isdigit(command) ? command - '0' : command - 'a' + 10;
  return bef_stack_push(state, digit);
}

static int bef_op_add(bef_state_t *state) {
  int a, b;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;
  return bef_stack_push(state, a + b);
}

static int bef_op_sub(bef_state_t *state) {
  int a, b;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;
  return bef_stack_push(state, b - a);
}

static int bef_op_mul(bef_state_t *state) {
  int a, b;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;
  return bef_stack_push(state, a * b);
}

static int bef_op_div(bef_state_t *state) {
  int a, b, sign;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;

  sign = a < 0 != b < 0 ? -1 : 1;
  if (a < 0) a *= -1;
  if (b < 0) b *= -1;
  return bef_stack_push(state, sign * (b / a));
}

static int bef_op_mod(bef_state_t *state) {
  int a, b;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;

  if (a < 0) a *= -1;
  if (b < 0) b *= -1;
  return bef_stack_push(state, b % a);
}

static int bef_op_not(bef_state_t *state) {
  int value;
  if (bef_stack_pop(state, &value)) return 1;
  return bef_stack_push(state, !value);
}

static int bef_op_gt(bef_state_t *state) {
  int a, b;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;
  return bef_stack_push(state, b > a);
}

static int bef_op_look_right(bef_state_t *state) {
  state->ip_dir = DIR_RIGHT;
  return 0;
}

static int bef_op_look_left(bef_state_t *state) {
  state->ip_dir = DIR_LEFT;
  return 0;
}

static int bef_op_look_up(bef_state_t *state) {
  state->ip_dir = DIR_UP;
  return 0;
}

static int bef_op_look_down(bef_state_t *state) {
  state->ip_dir = DIR_DOWN;
  return 0;
}

static int bef_op_rand_branch(bef_state_t *state) {
  state->ip_dir = (dir_t) ((double) rand() / RAND_MAX * DIR_COUNT);
  return 0;
}

static int bef_op_horz_branch(bef_state_t *state) {
  int condition;
  if (bef_stack_pop(state, &condition)) return 1;
  state->ip_dir = condition ? DIR_LEFT : DIR_RIGHT;
  return 0;
}

static int bef_op_vert_branch(bef_state_t *state) {
  int condition;
  if (bef_stack_pop(state, &condition)) return 1;
  state->ip_dir = condition ? DIR_UP : DIR_DOWN;
  return 0;
}

static int bef_op_string_toggle(bef_state_t *state) {
  state->string_mode = !state->string_mode;
  return 0;
}

static int bef_op_dupe(bef_state_t *state) {
  int value;
  if (bef_stack_peek(state, &value)) return 1;
  return bef_stack_push(state, value);
}

static int bef_op_swap(bef_state_t *state) {
  int a, b;
  if (bef_stack_pop(state, &a)) return 1;
  if (bef_stack_pop(state, &b)) return 1;
  if (bef_stack_push(state, a)) return 1;
  return bef_stack_push(state, b);
}

static int bef_op_discard(bef_state_t *state) {
  int value;
  return bef_stack_pop(state, &value);
}

// TODO: check print error checks
static int bef_op_print_int(bef_state_t *state) {
  int value;
  if (bef_stack_pop(state, &value)) return 1;
  return printf("%d ", value) < 0;
}

static int bef_op_print_char(bef_state_t *state) {
  int value;
  if (bef_stack_pop(state, &value)) return 1;
  return putchar(value) == EOF;
}

static int bef_op_skip(bef_state_t *state) {
  return bef_move(state);
}

static int bef_op_put(bef_state_t *state) {
  int row, col, value;
  if (bef_stack_pop(state, &row)) return 1;
  if (bef_stack_pop(state, &col)) return 1;
  if (bef_stack_pop(state, &value)) return 1;
  return bef_source_set(state, row, col, value);
}

static int bef_op_get(bef_state_t *state) {
  int row, col, value;
  if (bef_stack_pop(state, &row)) return 1;
  if (bef_stack_pop(state, &col)) return 1;
  if (bef_source_get(state, row, col, &value)) return 1;
  return bef_stack_push(state, value);
}

static int bef_op_scan_int(bef_state_t *state) {
  int value, result;
  errno = 0;
  result = scanf("%d", &value);
  if (errno) return 1;
  if (result != 1) value = 0;
  return bef_stack_push(state, value);
}

static int bef_op_scan_char(bef_state_t *state) {
  int value, result;
  errno = 0;
  result = getchar();
  if (errno) return 1;
  value = result != EOF ? result : 0;
  if (bef_stack_push(state, value)) return 1;
}

static int bef_op_exit(bef_state_t *state) {
  state->is_running = false;
  return 0;
}

static int bef_op_error(bef_state_t *state) {
  if (bef_stack_push(state, errno)) return 1;
  errno = 0;
  return 0;
}
