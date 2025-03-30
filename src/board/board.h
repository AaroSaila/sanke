#ifndef BOARD_H_
#define BOARD_H_

typedef struct {
  int x;
  int y;
} boardInfo;

typedef struct {
  int xs;
  int xe;
  int ys;
  int ye;
} playableBoardInfo;

extern boardInfo brdInfo;

void setBoardBorders(char board[][brdInfo.x]);
void printBoard(char board[][brdInfo.x]);

#endif
