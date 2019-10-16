#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "list.h"

/** Initializes a new dynamic list.  Returns 0 iff successful. */
int list_init(list_t *list, size_t elsize) {
  list->length = 0;
  list->elsize = elsize;
  list->capacity = LIST_INITIAL_CAPACITY;
  list->buffer = malloc(elsize * list->capacity);
  return list->buffer == NULL;
}

/**
 * Sets data to an arbitrary index, extending the length and capacity
 * if necessary.  Returns 0 iff successful.
 */
int list_set(list_t *list, size_t index, void *data) {
  void *newbuf;
  size_t newcap;
  const size_t MAX_CAP =  SIZE_MAX / list->elsize;

  if (list == NULL || data == NULL || index >= MAX_CAP) {
    return 1;
  }

  if (index >= list->length) list->length = index + 1;
  if (list->length > list->capacity) {
    newcap = 2 * list->length;
    if (newcap > MAX_CAP) return 1;

    newbuf = realloc(list->buffer, newcap * list->elsize);
    if (newbuf == NULL) return 1;

    list->buffer = newbuf;
    list->capacity = newcap;
  }

  memcpy((char *) list->buffer + index * list->elsize, data, list->elsize);
  return 0;
}

/** Gets data from an arbitrary index in range.  Returns 0 iff successful. */
int list_get(list_t *list, size_t index, void *data) {
  if (list == NULL || data == NULL || index >= list->length) return 1;
  memcpy(data, (char *) list->buffer + index * list->elsize, list->elsize);
  return 0;
}

/** Destroys a dynamic list. */
void list_destroy(list_t *list) {
  list->length = 0;
  list->capacity = 0;
  free(list->buffer);
}
