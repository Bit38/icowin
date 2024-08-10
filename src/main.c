#include "cli.h"
#include "format/ico.h"
#include "utils.h"
#include <stdio.h>

int info_command(struct parsed_cli *cli) {
  for (int i = 0; i < vec_size(cli->args); ++i) {
    char *path = vec_get(cli->args, i);
    struct icon *ico = NULL;
    enum file_error code = open_icon(path, &ico);

    if (code != FILE_OP_OK) {
      printf("Failed to read %s: %s\n", path, file_error_str(code));
      continue;
    }

    printf("%s:\n", path);
    printf("   FORMAT: %s\n",
           (ico->format == ICON_FORMAT_ICO) ? "ICO (icon)" : "CUR (cursor)");
    printf("   IMAGE COUNT: %d\n", ico->img_count);

    for (int j = 0; j < vec_size(ico->images); ++j) {
      printf("   IMAGE %d:\n", j + 1);
      struct icon_image *img = vec_get(ico->images, j);
      printf("      SIZE: %dx%d\n", img->width, img->height);
      printf("      FORMAT: %s\n", (img->format == IMAGE_FORMAT_BMP)
                                       ? "BMP (bitmap)"
                                       : "PNG (Portable Network Graphics)");

      if (ico->format == ICON_FORMAT_ICO) {
        printf("      COLOR PLANES: %d\n", img->color_planes);
        printf("      BITS PER PIXEL: %d\n", img->bits_per_pixel);
      } else
        printf("      HOTSPOT: %dx%d\n", img->hotspotX, img->hotspotY);

      char human_size[10] = {0};
      printf("      DATA SIZE: %s\n",
             to_human(img->data_size, human_size, sizeof(human_size)));
    }
    deallocate_icon(ico);
  }
  return 0;
}

int main(int argc, char **argv) {
  struct parsed_cli args = parse_args(argc, argv);

  switch (args.command) {
  case COMMAND_INFO:
    return info_command(&args);
  default:
    return 1;
  }

  return 0;
}
