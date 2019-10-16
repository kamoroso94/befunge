#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#define LIST_INITIAL_CAPACITY 8

typedef struct {
  size_t length;
  size_t capacity;
  size_t elsize;
  void *buffer;
} list_t;

int list_init(list_t *list, size_t elsize);
int list_set(list_t *list, size_t index, void *data);
int list_get(list_t *list, size_t index, void *data);
void list_destroy(list_t *list);

#endif /* LIST_H */
