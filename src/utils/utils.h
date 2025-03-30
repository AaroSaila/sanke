#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>

#include "../globals.h"

extern boardInfo brdInfo;

int randomInt(const int start, const int end, const unsigned int seed);
int randomX(const clock_t initClock);
int randomY(const clock_t initClock);
void sleep_ms(const int ms);
void mallocError(const char* varName, const char* fileName, const char* functionName);

#endif  // UTILS_H_
