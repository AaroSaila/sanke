#include <stdio.h>
#include "../globals.h"

extern boardInfo brdInfo;

void setBoardBorders(char board[][brdInfo.x]) {
  for (int i = 0; i < brdInfo.y; i++) {
    for (int j = 0; j < brdInfo.x; j++) {
      if (i == 0 || i == brdInfo.y - 1)
        board[i][j] = '-';
      else if (j == 0 || j == brdInfo.x - 1)
        board[i][j] = '|';
      else
        board[i][j] = ' ';
    }
  }

  board[0][0] = '+';
  board[brdInfo.y - 1][0] = '+';
  board[0][brdInfo.x - 1] = '+';
  board[brdInfo.y - 1][brdInfo.x - 1] = '+';
}

void printBoard(char board[][brdInfo.x]) {
  for (int i = 0; i < brdInfo.y; i++) {
    for (int j = 0; j < brdInfo.x; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}
