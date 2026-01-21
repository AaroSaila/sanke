#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

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

// TODO: Make a general error logging function and macro
void mallocError(const char* varName, const char* fileName, const char* functionName) {
  printf("Ran out of memory to allocate to %s in %s/%s\n", varName, fileName, functionName);
  exit(1);
}

void sleep_ms(const unsigned int ms) {
  struct timespec ts =  {
    .tv_sec = ms / 1000
  };
  if (ts.tv_sec == 0) {
    ts.tv_nsec = ms * 1000000;
  } else {
    ts.tv_nsec = (ms - ts.tv_sec * 1000) * 1000000;
  }
  // printf("timespec: {\n");
  // printf("  tv_sec : %ld\n", ts.tv_sec);
  // printf("  tv_nsec: %ld\n", ts.tv_nsec);
  // printf("}\n");
  if (nanosleep(&ts, NULL) == -1) {
    fprintf(stderr, "ERROR: Failed to sleep. ERRNO: %d\n", errno);
  }
}
