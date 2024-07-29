#include <stdio.h>
#include "test_utils.h"
#include "../global_macros.h"

void printBoard(char board[BRD_SIZE_X][BRD_SIZE_Y]) {
  for (int i = 0; i < BRD_SIZE_X; i++) {
    for (int j = 0; j < BRD_SIZE_Y; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}
