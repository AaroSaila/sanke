#ifndef ARGS_H_
#define ARGS_H_

typedef struct {
  int width;
  int height;
  int sleep_ms;
} Arguments;

Arguments cmd_args(int argc, char** argv);

#endif // ARGS_H_
