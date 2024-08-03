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
  // Order head
  newTail->order = (order*) malloc(sizeof(order));
  newTail->order->dir = newTail->dir;
  newTail->order->delay = -1;
  // First order if exists
  if (tail->order->next != NULL)
    copyOrders(tail->order, newTail->order);


  switch (newTail->dir) {
    case 'w':
      newTail->x = tail->x;
      newTail->y = tail->y + 1;
      break;
    case 's':
      newTail->x = tail->x;
      newTail->y = tail->y - 1;
      break;
    case 'a':
      newTail->x = tail->x + 2;
      newTail->y = tail->y;
      break;
    case 'd':
      newTail->x = tail->x - 2;
      newTail->y = tail->y;
      break;
    default:
      printf("Invalid direction in func addSnakePart\n");
      exit(1);
  }

  newTail->next = NULL;

  tail->next = newTail;

  board[newTail->y][newTail->x] = newTail->visChar;
}

void pushOrder(order* head, char dir, int delay) {
  order* current = head;
  while (current->next != NULL)
    current = current->next;

  order* newOrder = (order*) malloc(sizeof(order));
  newOrder->dir = dir;
  newOrder->delay = delay;
  newOrder->next = NULL;

  current->next = newOrder;
}

void removeOrder(order* head) {
  order* newFirstOrder = head->next->next;
  free(head->next);
  head->next = newFirstOrder;
}

void addOrders(snakePart* head, char dir) {
  snakePart* current = head;
  int i = 1;
  while (1) {
    if (current->next == NULL)
      break;

    current = current->next;

    pushOrder(current->order, dir, i);

    i++;
  }
}

void copyOrders(order* srcHead, order* destHead) {
  if (srcHead->next == NULL) {
    printf("ERROR in copyOrders: no orders to copy\n");
    exit(1);
  }

  order* srcCurrent = srcHead;
  order* destCurrent = destHead;
  order* destPrev = destHead;

  while (srcCurrent->next != NULL) {
    srcCurrent = srcCurrent->next;
    destPrev = destCurrent;
    destCurrent = (order*) malloc(sizeof(order));
    destPrev->next = destCurrent;
    destCurrent->dir = srcCurrent->dir;
    destCurrent->delay = srcCurrent->delay + 1;
    destCurrent->next = NULL;
  }
}
