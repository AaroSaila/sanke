#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "args.h"

static void check_bounds(const char* arg_name, const int i, const int argc) {
    if (i + 1 >= argc) {
        printf("%s needs an argument. See: sanke --help\n", arg_name);
        exit(0);
    }
}

static void show_version() {
  printf("Sanke version %s\n", VERSION);
  exit(0);
}

static void show_help() {
  printf(
          "USAGE: sanke <options>\n\n"
          "Options:\n"
          "  --width,  -w: Set the width (10 - 50) of the board. Default 15.\n"
          "  --height, -h: Set the height (10 - 50) of the board. Default 15.\n"
          "  --speed,  -s: Set the speed (1 - 1000) of the game in squares per second. Default 7.\n"
          "  --help      : Display this help text.\n"
          );
  exit(0);
}

static void set_width(Arguments* args, char* width_str) {
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
    printf("Width must be between 10 and 50. Was %s.\n", height_str);
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
    .sleep_ms = 150,
  };

  for (int i = 1; i < argc; i++) {
    if (
        strcmp(argv[i], "--version") == 0
        || strcmp(argv[i], "-v") == 0
        )
    {
      show_version();

    } else if (
        strcmp(argv[i], "--help") == 0
        )
    {
      show_help();

    } else if (
        strcmp(argv[i], "--width") == 0
        || strcmp(argv[i], "-w") == 0
        )
    {
      check_bounds("width", i, argc);
      set_width(&args, argv[i + 1]);

    } else if (
        strcmp(argv[i], "--height") == 0
        || strcmp(argv[i], "-h") == 0
        )
    {
      check_bounds("height", i, argc);
      set_height(&args, argv[i + 1]);

    } else if (
        strcmp(argv[i], "--speed") == 0
        || strcmp(argv[i], "-s") == 0
        )
    {
      check_bounds("speed", i, argc);
      set_speed(&args, argv[i + 1]);

    }
  }

  return args;
}
