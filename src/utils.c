#include "utils.h"
#include <stdio.h>

char *file_error_str(enum file_error code) {
  switch (code) {
  case FILE_OP_OK:
    return "File operation completed succesfully";
  case FILE_OPEN_ERROR:
    return "File could not be opened";
  case FILE_INVALID_PARAM_ERROR:
    return "Internal error";
  case FILE_READ_ERROR:
    return "Unknown error occured";
  case FILE_EOF_ERROR:
    return "Unexpected EOF";
  case FILE_WRITE_ERROR:
    return "Could not write to the file";
  case FILE_OUT_OF_MEM:
    return "Out of memory";
  case FILE_INVALID_SIG_ERROR:
    return "File has invalid signature";
  case FILE_CORRUPTED_ERROR:
    return "File is corrupted";
  default:
    printf("Unrecheable!\n");
    exit(1);
  }
}

enum file_error file_error_detect(FILE *fp, char read_mode) {
  if (feof(fp)) {
    fclose(fp);
    return FILE_EOF_ERROR;
  }
  if (ferror(fp)) {
    fclose(fp);
    return read_mode ? FILE_READ_ERROR : FILE_WRITE_ERROR;
  }
  fclose(fp);
  return FILE_OP_OK;
}

char is_little_endian() {
  uint16_t val = 0x0001;
  uint8_t *bytes = (uint8_t *)&val;

  return bytes[0] == 0x01;
}

enum file_error read_u16(FILE *file, uint16_t *val) {
  if (file == NULL || val == NULL)
    return FILE_INVALID_PARAM_ERROR;

  uint8_t buf[sizeof(uint16_t)] = {0};
  if (fread(buf, sizeof(uint16_t), 1, file) != 1) {
    if (feof(file))
      return FILE_EOF_ERROR;
    return FILE_READ_ERROR;
  }

  if (!is_little_endian()) {
    uint8_t tmp = buf[0];
    buf[0] = buf[1];
    buf[1] = tmp;
  }

  *val = *(uint16_t *)buf;

  return FILE_OP_OK;
}

int read_u32(FILE *file, uint32_t *val) {
  if (file == NULL || val == NULL)
    return FILE_INVALID_PARAM_ERROR;

  uint8_t buf[sizeof(uint32_t)] = {0};
  if (fread(buf, sizeof(uint32_t), 1, file) != 1) {
    return FILE_READ_ERROR;
  }

  if (!is_little_endian()) {
    int tmp = buf[0];
    buf[0] = buf[3];
    buf[3] = tmp;
    tmp = buf[1];
    buf[1] = buf[2];
    buf[2] = tmp;
  }

  *val = *(uint32_t *)buf;

  return FILE_OP_OK;
}

struct vector *vec_new() {
  struct vector *vec = (struct vector *)malloc(sizeof(struct vector));
  if (vec == NULL)
    return NULL;
  memset(vec, 0, sizeof(struct vector));

  vec->size = 0;
  vec->capacity = 5;
  vec->data = malloc(sizeof(void *) * vec->capacity);
  if (vec->data == NULL) {
    free(vec);
    return NULL;
  }

  return vec;
}

void vec_destroy(struct vector *vec) {
  free(vec->data);
  free(vec);
}

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

char *to_human(uint32_t bytes, char *buf, size_t buf_size) {
  char *sizes[] = {"B", "KB", "MB", "GB"};
  int devided = 0;

  if (buf == NULL)
    return buf;

  while (bytes > 1024 && devided < 3) {
    bytes /= 1024;
    devided++;
  }

  snprintf(buf, buf_size, "%d%s", bytes, sizes[devided]);
  return buf;
}
