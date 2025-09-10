#ifndef BOARD_H_
#define BOARD_H_

#include "Snake.h"

typedef struct {
  size_t width;
  size_t height;
  size_t width_with_borders;
  size_t height_with_borders;
  char* squares;
} Board;

Board board_alloc(
    const int width,
    const int height
    );
void board_free(Board* board);
bool board_coords_out_of_bounds(Board* board, const size_t x, const size_t y);
void board_set_square(
    Board* board,
    const int x,
    const int y,
    const char ch
    );
void board_clear(Board* board);
void board_draw_snake(Board* board, Snake* snake);
void print_board(Board* board);

#endif
