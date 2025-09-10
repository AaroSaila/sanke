#include <stdio.h>

#include "BoardPiece.h"

void board_piece_print_info(BoardPiece* piece, char* name) {
  printf("%s: {\n", name);
  printf("  x: %d\n", piece->x);
  printf("  y: %d\n", piece->y);
  printf("  vis_char: %c\n", piece->vis_char);
  printf("}\n");
}

bool pieces_collide(BoardPiece* piece1, BoardPiece* piece2) {
  return piece1->x == piece2->x && piece1->y == piece2->y;
}
