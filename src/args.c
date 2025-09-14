#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "args.h"

extern char* version;

static void handle_version() {
  printf("Sanke version %s\n", version);
  exit(0);
}

static void set_width(Arguments* args, char* width_str) {
  printf("width_str: %s\n", width_str);
  int width = atoi(width_str);
  if (width < 10) {
    printf("Width must be between 10 and 50. Was %d.\n", width);
    exit(0);
  }

  args->width = width;
}

static void set_height(Arguments* args, char* height_str) {
  int height = atoi(height_str);
  if (height < 10) {
    printf("Width must be between 10 and 50. Was %d.\n", height);
    exit(0);
  }

  args->height = height;
}

static void set_speed(Arguments* args, char* speed_str) {
  int squares_per_second = atoi(speed_str);
  if (squares_per_second < 1 || squares_per_second > 1000) {
    printf("Speed must be between 1 and 1000. Was %d.\n", squares_per_second);
    exit(0);
  }

  args->sleep_ms = 1000 / squares_per_second;
}

Arguments cmd_args(int argc, char** argv) {
  Arguments args = {
    .width = 15,
    .height = 15,
    .sleep_ms = 150
  };

  for (int i = 1; i < argc; i++) {
    if (
        strcmp(argv[i], "--version") == 0
        || strcmp(argv[i], "-v") == 0
        )
    {
      handle_version();

    } else if (
        strcmp(argv[i], "--width") == 0
        || strcmp(argv[i], "-w") == 0
        )
    {
      set_width(&args, argv[i + 1]);

    } else if (
        strcmp(argv[i], "--height") == 0
        || strcmp(argv[i], "-h") == 0
        )
    {
      set_height(&args, argv[i + 1]);

    } else if (
        strcmp(argv[i], "--speed") == 0
        || strcmp(argv[i], "-s") == 0
        )
    {
      set_speed(&args, argv[i + 1]);
    }
  }

  return args;
}
