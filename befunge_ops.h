#ifndef BEFUNGE_OPS_H
#define BEFUNGE_OPS_H

#include "list.h"
#include "stack.h"

#define BEF_OP(OP) int bef_op_ ## OP(list_t *source_rows, stack_t *stack, size_t x, size_t y)

int bef_op_digit(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_add(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_sub(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_mul(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_div(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_mod(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_not(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_gt(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_right(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_left(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_up(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_down(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_rand_any(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_rand_hor(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_look_rand_ver(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_dupe(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_swap(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_discard(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_print_int(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_print_char(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_skip(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_put(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_get(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_scan_int(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_scan_char(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);
int bef_op_exit(list_t *source_rows, stack_t *stack, size_t *x, size_t *y);

#endif /* BEFUNGE_OPS_H */
