#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "config.h"
#include "Board.h"
#include "Snake.h"

#define MAT_INDEX(mat, w, i, j) (mat)[(j) + (w) * (i)]

Board board_alloc(const int width, const int height) {
  Board board = {};
  board.width = width * 2;
  board.height = height;

  board.width_with_borders = board.width + 2;
  board.height_with_borders = board.height + 2;
  board.squares = (char*) malloc(sizeof(char) * board.width_with_borders * board.height_with_borders);

  for (size_t i = 1; i < board.height_with_borders; i++) {
    for (size_t j = 1; j < board.width_with_borders; j++) {
      MAT_INDEX(board.squares, board.width_with_borders, i, j) = ' ';
    }
  }

  // Create border pattern
  const int width_with_borders_last_i = board.width_with_borders - 1;
  const int height_with_borders_last_i = board.height_with_borders - 1;
  // Vertical bars
  for (int i = 1; i < height_with_borders_last_i; i++) {
    MAT_INDEX(board.squares, board.width_with_borders, i, 0) = CHAR_BORDER_VER;
    MAT_INDEX(board.squares, board.width_with_borders, i, width_with_borders_last_i) = CHAR_BORDER_VER;
  }
  // Horizontal lines
  for (int j = 1; j < width_with_borders_last_i; j++) {
    MAT_INDEX(board.squares, board.width_with_borders, 0, j) = CHAR_BORDER_HOR;
    MAT_INDEX(board.squares, board.width_with_borders, height_with_borders_last_i, j) = CHAR_BORDER_HOR;
  }

  // Corners
  MAT_INDEX(board.squares, board.width_with_borders, 0, 0) = CHAR_BORDER_CORNER_TL;
  MAT_INDEX(board.squares, board.width_with_borders, height_with_borders_last_i, 0) = CHAR_BORDER_CORNER_BL;
  MAT_INDEX(board.squares, board.width_with_borders, 0, width_with_borders_last_i) = CHAR_BORDER_CORNER_TR;
  MAT_INDEX(board.squares, board.width_with_borders, height_with_borders_last_i, width_with_borders_last_i) = CHAR_BORDER_CORNER_BR;

  return board;
}

void board_free(Board* board) {
  free(board->squares);
}

bool board_coords_out_of_bounds(Board* board, const size_t x, const size_t y) {
  return x >= board->width_with_borders || y >= board->height_with_borders;
}

void board_set_square(
    Board* board,
    int x,
    int y,
    const char ch
    )
{
  x++;
  y++;
  if (board_coords_out_of_bounds(board, x, y)) {
    fprintf(stderr, "ERROR: Board coords out of bounds: x: %d y: %d\n", x, y);
    exit(EXIT_FAILURE);
  }
  // assert(board_coords_out_of_bounds(board, x, y));
  MAT_INDEX(board->squares, board->width_with_borders, y, x) = ch;
}

void board_clear(Board* board) {
  for (size_t i = 0; i < board->height; i++) {
    for (size_t j = 0; j < board->width; j++) {
      // printf("Clearing board: i: %zu j: %zu\n", i, j);
      board_set_square(board, j, i, ' ');
    }
  }
}

void board_draw_snake(Board* board, Snake* snake) {
  BoardPiece part = {};
  for (size_t i = 0; i < snake->length; i++) {
    part = snake_get_part(snake, i);
    board_set_square(board, part.x, part.y, part.vis_char);
  }
}

void print_board(Board* board) {
  for (size_t i = 0; i < board->height_with_borders; i++) {
    for (size_t j = 0; j < board->width_with_borders; j++) {
      printf("%c", MAT_INDEX(board->squares, board->width_with_borders, i, j));
    }
    printf("\n");
  }
}
