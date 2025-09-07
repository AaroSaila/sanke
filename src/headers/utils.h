#ifndef UTILS_H_
#define UTILS_H_

int randomInt(const int start, const int end, const unsigned int seed);
void mallocError(const char* varName, const char* fileName, const char* functionName);
void sleep_ms(const unsigned int ms);

#endif  // UTILS_H_
