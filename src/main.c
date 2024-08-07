#include "cli.h"
#include <stdio.h>

int main(int argc, char **argv) {
  struct parsed_cli args = parse_args(argc, argv);
  return 0;
}
