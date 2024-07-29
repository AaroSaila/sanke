#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "./global_macros.h"

//TEMP
#include "./test_utils/test_utils.h"

int main() {
  // Termios setup
  struct termios attr;

  tcgetattr(STDIN_FILENO, &attr);

  const struct termios ATTR_ORIG = attr;

  attr.c_lflag &= ~(ECHO | ICANON);
  attr.c_cc[VMIN] = 0;
  attr.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, 0,&attr);

  // Game board setup
  char board[BRD_SIZE_X][BRD_SIZE_Y];

  for (int i = 0; i < BRD_SIZE_X; i++) {
    for (int j = 0; j < BRD_SIZE_Y; j++) {
      if (
          (i > 0 && i < BRD_SIZE_X - 1)
          && (j == 0 || j == BRD_SIZE_Y - 1)
          ) {
        board[i][j] = '|';
      } else if (
          (i == 0 || i == BRD_SIZE_X - 1)
          && (j > 0 && j < BRD_SIZE_Y - 1)
        ) {
        board[i][j] = '-';
      } else {
        board[i][j] = ' ';   
      }
    }
  }

  board[0][0] = '+';
  board[BRD_SIZE_X - 1][0] = '+';
  board[0][BRD_SIZE_Y - 1] = '+';
  board[BRD_SIZE_X - 1][BRD_SIZE_Y - 1] = '+';

  printBoard(board);

  /*
  while (1) {
    fflush(stdout);
    char buf[BUF_SIZE] = {0};
    read(STDIN_FILENO, buf, 2);

    if (buf[0] == 0)
      continue;

    write(STDOUT_FILENO, buf, 2);
    //printf("\n");
  }
  */

  // Termios reset
  tcsetattr(STDIN_FILENO, 0,&ATTR_ORIG);

  return 0;
}
