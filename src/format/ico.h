#ifndef ICO_H
#define ICO_H

#define _CRT_SECURE_NO_WARNINGS

#include "../utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum icon_format { ICON_FORMAT_ICO, ICON_FORMAT_CUR };
enum image_format { IMAGE_FORMAT_PNG, IMAGE_FORMAT_BMP };

struct icon_image {
  uint8_t width, height;

  union {
    uint16_t color_planes;
    uint16_t hotspotX;
  };

  union {
    uint16_t bits_per_pixel;
    uint16_t hotspotY;
  };

  enum image_format format;
  uint32_t data_size;
  uint8_t *data;
};

struct icon {
  enum icon_format format;

  uint16_t img_count;
  struct vector *images;
};

enum file_error open_icon(char *filename, struct icon **icon_struct);
void deallocate_icon(struct icon *icon_struct);

#endif // ICO_H
