#ifndef SNAKE_H_
#define SNAKE_H_

#include "../globals.h"

typedef struct snakeNode {
  int x;
  int y;
  char visChar;

  char dir;
  struct snakeNode* next;
} snakePart;

void addSnakePart(snakePart* head);

#endif  // SNAKE_H_
