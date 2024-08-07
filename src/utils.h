#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

/***************************************
 * Vector
 ***************************************/
struct vector {
  size_t size, capacity;
  void **data;
};

struct vector *vec_new();
void vec_destroy(struct vector *vec);

#define vec_size(vec) ((vec)->size)
#define vec_capacity(vec) ((vec)->capacity)
#define vec_get(vec, idx) ((vec)->data[idx])

int vec_add(struct vector *vec, void *item);

#endif // UTILS_H
