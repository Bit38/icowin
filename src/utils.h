#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum file_error {
  FILE_OP_OK,
  FILE_OPEN_ERROR,
  FILE_INVALID_PARAM_ERROR,
  FILE_READ_ERROR,
  FILE_EOF_ERROR,
  FILE_WRITE_ERROR,
  FILE_OUT_OF_MEM,
  FILE_INVALID_SIG_ERROR,
  FILE_CORRUPTED_ERROR
};

char *file_error_str(enum file_error code);
enum file_error file_error_detect(FILE *f, char read_mode);

char is_little_endian();
int read_u16(FILE *file, uint16_t *val);
int read_u32(FILE *file, uint32_t *val);

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
