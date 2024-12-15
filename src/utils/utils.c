#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

extern boardInfo brdInfo;
extern playableBoardInfo plBrdInfo;

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
    x = randomInt(plBrdInfo.xs, plBrdInfo.xe, i);
    if (x % 2 != 0)
      break;
  }

  return x;
}

int randomY(const clock_t initClock) {
  const unsigned int seed = clock() - initClock;
  return randomInt(plBrdInfo.ys, plBrdInfo.ye, seed);
}

void sleep_ms(const int ms) {
  usleep(ms * 1000);
}

void printBoard(char board[][brdInfo.x]) {
  for (int i = 0; i < brdInfo.y; i++) {
    for (int j = 0; j < brdInfo.x; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}
