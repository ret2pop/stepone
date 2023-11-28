#include <sopp.h>

#include <stdbool.h>
#include <stdlib.h>

sopp_t *init_sopp(char *source) {
  sopp_t *pp = malloc(sizeof(sopp_t));
  return pp;
}

char *sopp_process_all(sopp_t *pp) {
  bool flag = false;
  while (pp->source[pp->i] != '\0') {
    if (pp->source[pp->i] == '\n') {
      flag = true;
    }
    if (flag && pp->source[pp->i] == '#') {
    }
  }
  return pp->newstring;
}
