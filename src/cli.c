#include "cli.h"
#include <stdio.h>
#include <string.h>

void usage(char *prog) {
  printf("Usage: %s <command> [options]\n", prog);
  printf("Tool for working with ico, cur and ani files\n\n");
  printf("COMMANDS:\n");
  printf("  info <file1> [<file2> ...]    Prints information about provided "
         "files\n\n");
  printf("OPTIONS:\n");
  printf("  -V/--version                  Displays program version\n");
  printf("  -h/--help                     Displays this message\n");
}

struct parsed_cli parse_args(int argc, char **argv) {
  struct parsed_cli parsed;
  parsed.command = COMMAND_MISSING;
  parsed.prog = argv[0];
  parsed.args = vec_new();

  if (parsed.args == NULL) {
    fprintf(stderr, "Error: Out of memory!\n");
    exit(1);
  }

  if (argc < 2) {
    usage(parsed.prog);
    printf("Error: No command provided!\n");
    exit(1);
  }

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        usage(parsed.prog);
        exit(0);
      } else if (strcmp(argv[i], "-V") == 0 ||
                 strcmp(argv[i], "--version") == 0) {
        printf("icowin version " VERSION "\n");
        exit(0);
      } else {
        fprintf(stderr, "Warning: Unknown option: `%s`. Ignoring.\n", argv[i]);
      }
      continue;
    }
    if (parsed.command == COMMAND_MISSING) {
      if (strcmpi(argv[i], "info") == 0)
        parsed.command = COMMAND_INFO;
      else {
        usage(parsed.prog);
        printf("Error: Invalid command: %s\n", argv[i]);
        exit(1);
      }
      continue;
    }
    vec_add(parsed.args, argv[i]);
  }

  switch (parsed.command) {
  case COMMAND_INFO:
    if (vec_size(parsed.args) <= 0) {
      usage(parsed.prog);
      printf("Error: No file provided\n");
      exit(1);
    }
    break;
  case COMMAND_MISSING:
    usage(parsed.prog);
    printf("Error: No command provided");
    exit(1);
  }

  return parsed;
}
