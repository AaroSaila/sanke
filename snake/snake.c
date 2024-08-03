#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "snake.h"

extern char board[BRD_SIZE_Y][BRD_SIZE_X];
extern char SNAKE_VIS;

void mvSnakeParts(snakePart* head) { 
  snakePart* part = head;
  while (1) {

    order* orderHead = part->order;

    if (orderHead->next != NULL) {
      if (orderHead->next->delay == 0) {
        part->dir = orderHead->next->dir;
        removeOrder(orderHead);
      }
    }

    if (orderHead->next != NULL) {
      order* current = orderHead->next;
      while (1) {
        if (current->delay > 0)
          current->delay--;
        if (current->next == NULL)
          break;
        current = current->next;
      }
    }

    int x = part->x;
    int y = part->y;

    board[y][x] = ' ';

    switch (part->dir) {
      case 'w':
        y = y - 1 < PL_BRD_YS ? PL_BRD_YE : y - 1;
        break;
      case 's':
        y = y + 1 > PL_BRD_YE ? PL_BRD_YS : y + 1;
        break;
      case 'a':
        x = x - 2 < PL_BRD_XS ? PL_BRD_XE - 1 : x - 2;
        break;
      case 'd':
        x = x + 2 > PL_BRD_XE ? PL_BRD_XS : x + 2;
        break;
      default:
        printf("ERROR in func mvSnakeParts\n");
        printf("dir: %c\n", part->dir);
        exit(1);
    }

    part->x = x;
    part->y = y;

    board[y][x] = part->visChar;

    if (part->next == NULL)
      return;

    part = part->next;
  }
}

void addSnakePart(snakePart* head) {
  snakePart* tail = head;

  while (tail->next != NULL)
    tail = tail->next;

  snakePart* newTail = (snakePart*) malloc(sizeof(snakePart));
  newTail->visChar = SNAKE_VIS;
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

bool checkCollision(snakePart* head, int x, int y) {
  snakePart* current = head;
  while (1) {
    if (current->x == x && current->y == y)
      return true;

    if (current->next == NULL)
      break;

    current = current->next;
  }

  return false;
}
