#include "utils.h"

struct vector *vec_new() {
  struct vector *vec = (struct vector *)malloc(sizeof(struct vector));
  if (vec == NULL)
    return NULL;

  vec->size = 0;
  vec->capacity = 5;
  vec->data = malloc(sizeof(void *) * vec->capacity);
  if (vec->data == NULL) {
    free(vec);
    return NULL;
  }

  return vec;
}

void vec_destroy(struct vector *vec) { free(vec); }

int vec_add(struct vector *vec, void *item) {
  if (vec == NULL)
    return -1;

  if (vec->size == vec->capacity) {
    vec->capacity += 5;
    void **new_data = realloc(vec->data, sizeof(void *) * vec->capacity);
    if (new_data == NULL)
      return -1;
    vec->data = new_data;
  }

  vec->data[vec->size++] = item;
  return 0;
}
