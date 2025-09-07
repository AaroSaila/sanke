#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"

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

void mallocError(const char* varName, const char* fileName, const char* functionName) {
  printf("Ran out of memory to allocate to %s in %s/%s\n", varName, fileName, functionName);
  exit(1);
}

void sleep_ms(const unsigned int ms) {
  nanosleep(&(struct timespec){ .tv_sec = 0, .tv_nsec = ms * 1000000}, NULL);
}
