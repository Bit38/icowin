#include "ico.h"

enum file_error open_icon(char *filename, struct icon **icon_struct) {
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
    return FILE_OPEN_ERROR;

  enum file_error err_code = FILE_OP_OK;
  struct icon *ico = *icon_struct;

  uint16_t buf;
  if (fread(&buf, sizeof(uint16_t), 1, fp) != 1) {
    err_code = file_error_detect(fp, 1);
    goto exit_func;
  }
  if (buf != 0) {
    err_code = FILE_INVALID_SIG_ERROR;
    goto exit_func;
  }

  if (*icon_struct)
    deallocate_icon(*icon_struct);

  *icon_struct = malloc(sizeof(struct icon));
  if (*icon_struct == NULL) {
    err_code = FILE_OUT_OF_MEM;
    goto exit_func;
  }
  ico = *icon_struct;
  memset(ico, 0, sizeof(struct icon));

  if ((err_code = read_u16(fp, &buf)) != FILE_OP_OK)
    goto exit_func;

  if (buf == 1) {
    ico->format = ICON_FORMAT_ICO;
  } else if (buf == 2) {
    ico->format = ICON_FORMAT_CUR;
  }

  if ((err_code = read_u16(fp, &ico->img_count)) != FILE_OP_OK)
    goto exit_func;

  if (ico->img_count == 0) {
    err_code = FILE_CORRUPTED_ERROR;
    goto exit_func;
  }

  if ((ico->images = vec_new()) == NULL) {
    err_code = FILE_OUT_OF_MEM;
    goto exit_func;
  }

  for (uint16_t i = 0; i < ico->img_count; ++i) {
    struct icon_image *img = malloc(sizeof(struct icon_image));
    if (img == NULL) {
      err_code = FILE_OUT_OF_MEM;
      goto exit_func;
    }

    if (fread(&img->width, sizeof(uint8_t), 1, fp) != 1) {
      err_code = file_error_detect(fp, 1);
      goto exit_func;
    }

    if (fread(&img->height, sizeof(uint8_t), 1, fp) != 1) {
      err_code = file_error_detect(fp, 1);
      goto exit_func;
    }

    if (fseek(fp, 2, SEEK_CUR) != 0) {
      err_code = FILE_READ_ERROR;
      goto exit_func;
    }

    if ((err_code = read_u16(fp, &img->color_planes)) != FILE_OP_OK)
      goto exit_func;

    if ((err_code = read_u16(fp, &img->bits_per_pixel)) != FILE_OP_OK)
      goto exit_func;

    if ((err_code = read_u32(fp, &img->data_size)) != FILE_OP_OK)
      goto exit_func;

    uint32_t offset_data;
    if ((err_code = read_u32(fp, &offset_data)) != FILE_OP_OK)
      goto exit_func;

    uint32_t offset_next_img = ftell(fp);
    if (fseek(fp, offset_data, SEEK_SET) != 0) {
      err_code = FILE_READ_ERROR;
      goto exit_func;
    }

    img->data = malloc(sizeof(uint8_t) * img->data_size);
    if (img->data == NULL) {
      err_code = FILE_OUT_OF_MEM;
      goto exit_func;
    }

    if (fread(img->data, sizeof(uint8_t), img->data_size, fp) !=
        img->data_size) {
      err_code = file_error_detect(fp, 1);
      goto exit_func;
    }

    img->format = (memcmp(img->data, PNG_SIG, 8) == 0) ? IMAGE_FORMAT_PNG
                                                       : IMAGE_FORMAT_BMP;

    if (fseek(fp, offset_next_img, SEEK_SET) != 0) {
      err_code = FILE_READ_ERROR;
      goto exit_func;
    }
    vec_add(ico->images, img);
  }

  fclose(fp);
  return FILE_OP_OK;

exit_func:
  deallocate_icon(ico);
  fclose(fp);
  return err_code;
}

void deallocate_icon(struct icon *icon_struct) {
  if (icon_struct == NULL)
    return;
  if (icon_struct->images != NULL) {
    for (int i = 0; i < vec_size(icon_struct->images); ++i) {
      struct icon_image *img = vec_get(icon_struct->images, i);
      if (img != NULL)
        free(img->data);
      free(img);
    }
  }
  vec_destroy(icon_struct->images);
  free(icon_struct);
}
