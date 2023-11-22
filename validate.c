#include "validate.h"
#include "parser.h"

#include <stdlib.h>

valid_t *init_validator(parser_t *p) {
  valid_t *v = malloc(sizeof(valid_t));
  v->root = parse_all(p);
  return v;
}
