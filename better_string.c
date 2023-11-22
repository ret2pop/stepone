#include "better_string.h"
#include "macros.h"
#include <stdlib.h>
#include <string.h>

string_t *init_string(char *s) {
  string_t *str = malloc(sizeof(string_t));
  if (str == NULL)
    die("malloc in init_string");
  /* If we pass in NULL we assume that we want to append things to this in the
   * future */
  if (s == NULL) {
    str->bufsize = 10;
    str->value = calloc(str->bufsize, sizeof(char));
    if (str->value == NULL)
      die("calloc in init_string");
    str->length = 0;
    return str;
  }
  /* Otherwise we want the initial value to be equal to the parameter */
  str->length = strlen(s);
  str->bufsize = 2 * strlen(s);
  str->value = calloc(str->bufsize, sizeof(char));
  if (str->value == NULL)
    die("calloc in init_string");
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
