#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "list.h"

typedef struct {
  list_t list;
} stack_t;

int stack_init(stack_t *stack, size_t elsize);
int stack_push(stack_t *stack, void *data);
int stack_pop(stack_t *stack, void *data);
int stack_peek(stack_t *stack, void *data);
void stack_destroy(stack_t *stack);

#endif /* STACK_H */
