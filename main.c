#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "./globals.h"

//TEMP
#include "./test_utils/test_utils.h"

typedef struct {
  int x;
  int y;
  char visChar;
} brdSymbol;

int randomInt(const int start, const int end);
void sleep_ms(const int ms);
void mvSymbol(brdSymbol* symbol, const char dir);

char board[BRD_SIZE_Y][BRD_SIZE_X];

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
  for (int i = 0; i < BRD_SIZE_Y; i++) {
    for (int j = 0; j < BRD_SIZE_X; j++) {
      if (
          (i > 0 && i < BRD_SIZE_Y - 1)
          && (j == 0 || j == BRD_SIZE_X - 1)
          ) {
        board[i][j] = '|';
      } else if (
          (i == 0 || i == BRD_SIZE_Y - 1)
          && (j > 0 && j < BRD_SIZE_X - 1)
        ) {
        board[i][j] = '-';
      } else {
        board[i][j] = ' ';   
      }
    }
  }

  board[0][0] = '+';
  board[BRD_SIZE_Y - 1][0] = '+';
  board[0][BRD_SIZE_X - 1] = '+';
  board[BRD_SIZE_Y - 1][BRD_SIZE_X - 1] = '+';

  brdSymbol player;
  player.x = randomInt(PL_BRD_XS, PL_BRD_XE);
  player.y = randomInt(PL_BRD_YS, PL_BRD_YE);
  player.visChar = '#';

  board[player.y][player.x] = player.visChar;

  printBoard();

  while (1) {
    fflush(stdout);
    char buf[1] = {0};
    read(STDIN_FILENO, buf, 1);
    char input = buf[0];

    if (input == 0 || !isalpha(input))
      continue;

    input = tolower(input);
    
    switch (input) {
      case 'w':
        mvSymbol(&player, 'u');
        break;
      case 's':
        mvSymbol(&player, 'd');
        break;
      case 'a':
        mvSymbol(&player, 'l');
        break;
      case 'd':
        mvSymbol(&player, 'r');
        break;
      default:
        NULL;
    }
  
    system("clear");
    printBoard();
    printf("x: %d\n", player.x);
    printf("y: %d\n", player.y);

    sleep_ms(50);
  }

  // Termios reset
  tcsetattr(STDIN_FILENO, 0,&ATTR_ORIG);

  return 0;
}

int randomInt(const int start, const int end) {
  /*
   * Gets random int from range
   * [start, end[
  */
  srand(time(NULL));
  int result = rand() % end;
  result = result >= start ? result : result + start;
  return result;
}

void sleep_ms(const int ms) {
  usleep(ms * 1000);
}

void mvSymbol(brdSymbol* symbol, const char dir) { 
  board[symbol->y][symbol->x] = ' ';

  switch (dir) {
    case 'u':
      symbol->y = symbol->y - 1 < PL_BRD_YS ? PL_BRD_YE : symbol->y - 1;
      break;
    case 'd':
      symbol->y = symbol->y + 1 > PL_BRD_YE ? PL_BRD_YS : symbol->y + 1;
      break;
    case 'l':
      symbol->x = symbol->x - 2 < PL_BRD_XS ? PL_BRD_XE : symbol->x - 2;
      break;
    case 'r':
      symbol->x = symbol->x + 2 > PL_BRD_XE ? PL_BRD_XS : symbol->x + 2;
      break;
    default:
      printf("ERROR in func mvSymbol\n");
      exit(1);
  }

  board[symbol->y][symbol->x] = symbol->visChar;
}
