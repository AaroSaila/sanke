#ifndef SNAKE_H_
#define SNAKE_H_

#include "../globals.h"

extern boardInfo brdInfo;

typedef struct orderNode {
  char dir;
  int delay;
  struct orderNode* next;
} order;

typedef struct snakeNode {
  int x;
  int y;
  char visChar;

  char dir;
  order* order;
  struct snakeNode* next;
} snakePart;

void mvSnakeParts(char board[][brdInfo.x], snakePart* head);
void addSnakePart(char board[][brdInfo.x], snakePart* head);
void pushOrder(order* head, char dir, int delay);
void removeOrder(order* head);
void addOrders(snakePart* head, char dir);
void copyOrders(order* srcHead, order* destHead);
bool checkCollision(snakePart* head, int x, int y);

#endif  // SNAKE_H_
