#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

extern char board[BRD_SIZE_Y][BRD_SIZE_X];

void addSnakePart(snakePart* head) {
  snakePart* tail = head;

  while (tail->next != NULL)
    tail = tail->next;

  snakePart* newTail = (snakePart*) malloc(sizeof(snakePart));
  newTail->visChar = '#';
  newTail->dir = tail->dir;

  switch (newTail->dir) {
    case 'w':
      newTail->x = tail->x;
      newTail->y = tail->y + 1;
      break;
    case 's':
      newTail->x = tail->x - 1;
      newTail->y = tail->y;
      break;
    case 'a':
      newTail->x = tail->x - 1;
      newTail->y = tail->y;
      break;
    case 'd':
      newTail->x = tail->x;
      newTail->y = tail->y - 1;
      break;
    default:
      printf("Invalid direction in func addSnakePart\n");
      exit(1);
  }

  newTail->next = NULL;

  tail->next = newTail;

  board[newTail->y][newTail->x] = newTail->visChar;
}
