#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "../globals.h"

extern char board[BRD_SIZE_Y][BRD_SIZE_X];

int randomInt(const int start, const int end, const unsigned int seed) {
  /*
   * Gets random int from range
   * [start, end[
  */
  srand(seed);
  int result = rand() % end;
  result = result >= start ? result : result + start;
  return result;
}

int randomX(const clock_t initClock) {
  int x;
  const unsigned int seed = clock() - initClock;
  for (int i = randomInt(0, 100, seed); ; i++) {
    x = randomInt(PL_BRD_XS, PL_BRD_XE, i);
    if (x % 2 == 0)
      break;
  }

  return x;
}

int randomY(const clock_t initClock) {
  const unsigned int seed = clock() - initClock;
  return randomInt(PL_BRD_YS, PL_BRD_YE, seed);
}

void sleep_ms(const int ms) {
  usleep(ms * 1000);
}

void printBoard() {
  for (int i = 0; i < BRD_SIZE_Y; i++) {
    for (int j = 0; j < BRD_SIZE_X; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}
