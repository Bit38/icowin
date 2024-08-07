#ifndef CLI_H
#define CLI_H

#define _CRT_NONSTDC_NO_WARNINGS

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.1"

enum cli_command { COMMAND_INFO, COMMAND_MISSING };
struct parsed_cli {
  char *prog;
  enum cli_command command;
  struct vector *args;
};

void usage(char *prog);
struct parsed_cli parse_args(int argc, char **argv);

#endif // CLI_H
