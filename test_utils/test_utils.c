#include <stdio.h>
#include "test_utils.h"
#include "../globals.h"

extern char board[BRD_SIZE_Y][BRD_SIZE_X];

void printBoard() {
  for (int i = 0; i < BRD_SIZE_Y; i++) {
    for (int j = 0; j < BRD_SIZE_X; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}
