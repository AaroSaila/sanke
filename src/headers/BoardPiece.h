#ifndef BOARD_PIECE_H_
#define BOARD_PIECE_H_

typedef struct {
  int x;
  int y;
  char vis_char;
} BoardPiece;

void board_piece_print_info(BoardPiece* piece, char* name);
bool pieces_collide(BoardPiece* piece1, BoardPiece* piece2);

#endif // BOARD_PIECE_H_
