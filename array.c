#include "array.h"
#include <stdlib.h>

array_t *init_array(size_t size) {
  array_t *a = malloc(sizeof(array_t *));
  a->items = calloc(size, sizeof(void *));
  a->size = size;
  return a;
}
