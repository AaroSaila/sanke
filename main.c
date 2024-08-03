#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "./globals.h"
#include "./utils/utils.h"
#include "./snake/snake.h"

char board[BRD_SIZE_Y][BRD_SIZE_X];

const char SNAKE_VIS = '#';

int main() {
  clock_t initClock = clock();

  // Termios setup
  struct termios attr;

  tcgetattr(STDIN_FILENO, &attr);

  const struct termios ATTR_ORIG = attr;

  attr.c_lflag &= ~(ECHO | ICANON);
  attr.c_cc[VMIN] = 0;
  attr.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, 0, &attr);

  // Game board setup
  for (int i = 0; i < BRD_SIZE_Y; i++) {
    for (int j = 0; j < BRD_SIZE_X; j++) {
      if (i == 0 || i == BRD_SIZE_Y - 1)
        board[i][j] = '-';
      else if (j == 0 || j == BRD_SIZE_X - 1)
        board[i][j] = '|';
      else
        board[i][j] = ' ';
    }
  }

  board[0][0] = '+';
  board[BRD_SIZE_Y - 1][0] = '+';
  board[0][BRD_SIZE_X - 1] = '+';
  board[BRD_SIZE_Y - 1][BRD_SIZE_X - 1] = '+';

  int points = 0;
  int gameSpeed = 0;
  int sleepInterval = 200 - gameSpeed;

  // Snake head setup
  snakePart* snakeHead = (snakePart*) malloc(sizeof(snakePart));
  snakeHead->x = randomX(initClock);
  snakeHead->y = randomY(initClock);
  snakeHead->visChar = '&';
  snakeHead->dir = 'w';
  snakeHead->order = (order*) malloc(sizeof(order));
  snakeHead->order->dir = snakeHead->dir;
  snakeHead->order->delay = -1;
  snakeHead->order->next = NULL;
  snakeHead->next = NULL;

  board[snakeHead->y][snakeHead->x] = snakeHead->visChar;

  // Food setup
  struct {
    int x;
    int y;
    char visChar;
  } food;
  food.x = randomX(initClock);
  food.y = randomY(initClock);
  food.visChar = '$';

  board[food.y][food.x] = food.visChar;

  // Screen init
  system("clear");

  // Game loop
  while (1) {
    fflush(stdout);
    char buf[1] = {0};

    // Food collision
    if (snakeHead->x == food.x
        && snakeHead->y == food.y) {
      points++;
      if (points % 5 == 0)
        gameSpeed += 15;
      do {
        food.x = randomX(initClock);
        food.y = randomY(initClock);
      } while (checkCollision(snakeHead, food.x, food.y));
      addSnakePart(snakeHead);
    }

    // Input handling
    if (read(STDIN_FILENO, buf, 1) != 0 && isalpha(buf[0])) {
      char input = buf[0];
      input = tolower(input);

      if (
          (input == 'w' && snakeHead->dir != 's')
          || (input == 's' && snakeHead->dir != 'w')
          || (input == 'a' && snakeHead->dir != 'd')
          || (input == 'd' && snakeHead->dir != 'a')
          ) {
        snakeHead->dir = input;
        addOrders(snakeHead, input);
      }
    }

    // Update board
    board[food.y][food.x] = food.visChar;
    mvSnakeParts(snakeHead);

    // Snake collision
    if (snakeHead->next != NULL) {
      snakePart* current = snakeHead->next;
      while (1) {
        if (current->x == snakeHead->x
            && current->y == snakeHead->y)
          goto game_over;

        if (current->next == NULL)
          break;

        current = current->next;
      }
    }

    system("clear");
    printBoard();
    printf("Points: %d\n", points);
    printf("Game Speed: %d\n", gameSpeed);

    /*
    printf("x: %d\n", snakeHead->x);
    printf("y: %d\n\n", snakeHead->y);
    printf("food x: %d\n", food.x);
    printf("food y: %d\n", food.y);

    {
      snakePart* current = snakeHead;
      int i = 0;
      while (1) {
        printf("Part %d:\n", i);
        printf("dir: %c\n", current->dir);
        printf("orders: \n");
        order* order = current->order;
        int order_i = 0;
        while (1) {
          printf("\t\nOrder %d:\n", order_i);
          printf("\tdir: %c\n", order->dir);
          printf("\tdelay: %d\n", order->delay);
          printf("\tnext: ");
          if (order->next == NULL)
            printf("NULL\n");
          else
            printf("exists\n");
          
          if (order->next == NULL)
            break;

          order = order->next;

          order_i++;
        }
        if (current->next == NULL)
          break;

        current = current->next;
        i++;
      }
    }
    */

    sleep_ms(sleepInterval - gameSpeed);
  }

game_over:

  // Game over
  system("clear");

  printBoard();
  printf("Final Points: %d\n", points);
  printf("Final Game Speed: %d\n", gameSpeed);

  tcsetattr(STDIN_FILENO, 0, &ATTR_ORIG);

  return 0;
}
