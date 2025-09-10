#ifndef SNAKE_H_
#define SNAKE_H_

#include <stddef.h>
#include "BoardPiece.h"

typedef struct {
  BoardPiece* parts;
  size_t max_length;
  size_t length;
  char dir;
} Snake;

BoardPiece snake_get_part(const Snake* snake, const size_t index);
Snake snake_alloc(
    const int board_square_count,
    const int init_x,
    const int init_y,
    const char init_dir
    );
void snake_free(Snake* snake);
void snake_move(Snake* snake, const int width, const int height);
void snake_print_info(Snake* snake);
void snake_change_direction(Snake* snake, const char direction);
bool snake_collides(const Snake* snake, const BoardPiece* piece);
bool snake_collides_with_tail(const Snake* snake);
void snake_add_part(Snake* snake);

#endif // SNAKE_H_
