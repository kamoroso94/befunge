#include <stdlib.h>
#include "list.h"
#include "stack.h"

/** Initialize an empty stack.  Returns 0 iff successful. */
int stack_init(stack_t *stack, size_t elsize) {
  if (stack == NULL) return 1;
  return list_init(&stack->list, elsize);
}

/** Push data onto the stack.  Returns 0 iff successful. */
int stack_push(stack_t *stack, void *data) {
  if (stack == NULL) return 1;
  return list_set(&stack->list, stack->list.length, data);
}

/** Pop data off of the stack.  Returns 0 iff successful. */
int stack_pop(stack_t *stack, void *data) {
  void *newbuf;
  size_t newcap;

  if (stack_peek(stack, data)) return 1;
  stack->list.length--;

  // shrink capacity when length falls below 25%
  if (stack->list.capacity > LIST_INITIAL_CAPACITY &&
      stack->list.length < stack->list.capacity / 4) {
    newcap = stack->list.capacity / 2;
    stack->list.capacity = newcap > LIST_INITIAL_CAPACITY ?
        newcap : LIST_INITIAL_CAPACITY;

    newbuf = realloc(stack->list.buffer,
        stack->list.capacity * stack->list.elsize);
    if (newbuf == NULL) return 1;
    stack->list.buffer = newbuf;
  }

  return 0;
}

/** Peek at the data at the top of the stack.  Returns 0 iff successful. */
int stack_peek(stack_t *stack, void *data) {
  if (stack == NULL || stack->list.length == 0) return 1;
  return list_get(&stack->list, stack->list.length - 1, data);
}

/** Destroy the stack. */
void stack_destroy(stack_t *stack) {
  if (stack == NULL) return;
  stack_destroy(&stack->list);
}
