#include "better_string.h"
#include <stdlib.h>
#include <string.h>

string_t *init_string(char *s) {
  string_t *str = malloc(sizeof(string_t));
  if (s == NULL) {
    str->bufsize = 10;
    str->value = calloc(str->bufsize, sizeof(char));
    str->length = 0;
    return str;
  }
  str->length = strlen(s);
  str->bufsize = 2 * strlen(s);
  str->value = malloc(str->bufsize);
  strcpy(str->value, s);
  return str;
}

string_t *string_copy(string_t *s) { return init_string(s->value); }

void string_append(string_t *s, char c) {
  char str[2] = {c, '\0'};
  if (s->bufsize == s->length) {
    s->bufsize = s->bufsize * 2;
  }
  s->length++;
  strcat(s->value, str);
}

void string_free(string_t *s) {
  free(s->value);
  free(s);
}
