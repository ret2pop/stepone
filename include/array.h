#ifndef ARRAY_H
#define ARRAY_H
#include <stdlib.h>

/* We use this for storing AST subnodes */
typedef struct {
  void **items;
  size_t size;
} array_t;

/* Allocate memory for new array */
array_t *init_array(size_t size);

/* Add to back of array */
void array_append(array_t *a);

/* Get array value at index */
void *array_get(array_t *a, int i);
#endif
