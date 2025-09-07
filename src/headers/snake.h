#ifndef SNAKE_H_
#define SNAKE_H_

#include <stddef.h>

typedef struct {
  int x;
  int y;
  char vis_char;
} SnakePart;

typedef struct {
  SnakePart* parts;
  size_t max_length;
  size_t length;
  char dir;
} Snake;

SnakePart snake_get_part(const Snake* snake, const size_t index);
Snake snake_alloc(
    const int board_square_count,
    const int init_x,
    const int init_y,
    const char init_dir
    );
void snake_free(Snake* snake);
void snake_move(Snake* snake);

#endif // SNAKE_H_
