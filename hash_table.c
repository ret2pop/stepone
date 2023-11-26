#include "hash_table.h"
#include "ast.h"
#include "better_string.h"
#include <stdlib.h>
#include <string.h>

sl_node_t *init_sl_node(string_t *name, ast_t *value) {
  sl_node_t *n = malloc(sizeof(sl_node_t));
  n->name = name;
  n->value = value;
  n->next = NULL;
  return n;
}

sl_list_t *init_sl_list() {
  sl_list_t *l = malloc(sizeof(sl_list_t));
  l->head = NULL;
  l->size = 0;
  return l;
}

void sl_list_add(sl_list_t *l, string_t *name, ast_t *n) {
  sl_node_t *cur = l->head;
  if (l->head == NULL) {
    l->head = init_sl_node(string_copy(name), ast_copy(n));
  }
  while (cur->next != NULL && strcmp(cur->name->value, name->value) != 0) {
    cur = cur->next;
  }
  if (strcmp(cur->name->value, name->value) == 0) {
    ast_free(cur->value);
    cur->value = n;
  }
  cur->next = init_sl_node(string_copy(name), ast_copy(n));
}

ast_t *sl_list_get(sl_list_t *l, string_t *name) {
  sl_node_t *cur = l->head;
  while (cur != NULL) {
    if (strcmp(cur->name->value, name->value) == 0) {
      return cur->value;
    }
  }
  return NULL;
}

void sl_list_free(sl_list_t *l) {
  sl_node_t *cur = l->head;
  sl_node_t *tmp;
  while (cur != NULL) {
    string_free(cur->name);
    tmp = cur->next;
    free(cur);
    cur = tmp;
  }
  free(l);
}

hash_table_t *init_hash_table(size_t size) {
  hash_table_t *h = malloc(sizeof(hash_table_t));
  h->size = size;
  h->buckets = malloc(size * sizeof(sl_list_t));
  for (int i = 0; i < size; i++) {
    h->buckets[i] = init_sl_list();
  }
  return h;
}

void hash_table_add(hash_table_t *h, string_t *name, ast_t *value) {
  sl_list_add(h->buckets[hash(name->value) % h->size], name, value);
}

ast_t *hash_table_get(hash_table_t *h, string_t *name) {
  return sl_list_get(h->buckets[hash(name->value) % h->size], name);
}

void hash_table_free(hash_table_t *h) {
  for (int i = 0; i < h->size; i++) {
    sl_list_free(h->buckets[i]);
  }
  free(h);
}
/* djb2 hash function */
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
