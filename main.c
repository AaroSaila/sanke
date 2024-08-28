#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "./utils/utils.h"
#include "./snake/snake.h"
#include "globals.h"

const char SNAKE_VIS = '#';

boardInfo brdInfo;
playableBoardInfo plBrdInfo;

int main() {
  const clock_t initClock = clock();

  // Board Constraints
  printf("Set board size (15 - 60): ");
  scanf("%d", &brdInfo.y);
  if (!(brdInfo.y >= 15 && brdInfo.y <= 60)) {
    printf("Invalid input. Board size must be greater than 0.\n");
    exit(0);
  }
  brdInfo.x = brdInfo.y * 2;
  printf("brdInfo.x: %d\n", brdInfo.x);
  printf("brdInfo.y: %d\n", brdInfo.y);
  char board[brdInfo.y][brdInfo.x];

  plBrdInfo.xs = 1;
  plBrdInfo.xe = brdInfo.x - 2;
  plBrdInfo.ys = 1;
  plBrdInfo.ye = brdInfo.y - 2;


  // Termios setup
  struct termios attr;

  tcgetattr(STDIN_FILENO, &attr);

  const struct termios ATTR_ORIG = attr;

  attr.c_lflag &= ~(ECHO | ICANON);
  attr.c_cc[VMIN] = 0;
  attr.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, 0, &attr);

  // Game board setup
  for (int i = 0; i < brdInfo.y; i++) {
    for (int j = 0; j < brdInfo.x; j++) {
      if (i == 0 || i == brdInfo.y - 1)
        board[i][j] = '-';
      else if (j == 0 || j == brdInfo.x - 1)
        board[i][j] = '|';
      else
        board[i][j] = ' ';
    }
  }

  board[0][0] = '+';
  board[brdInfo.y - 1][0] = '+';
  board[0][brdInfo.x - 1] = '+';
  board[brdInfo.y - 1][brdInfo.x - 1] = '+';

  int points = 0;
  int gameSpeed = 0;
  const int sleepInterval = 200;

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
      addSnakePart(board, snakeHead);
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
    mvSnakeParts(board, snakeHead);

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
    printBoard(board);
    printf("Points: %d\n", points);
    printf("Game Speed: %d\n", gameSpeed);

    sleep_ms(sleepInterval - gameSpeed);
  }

game_over:

  // Game over
  system("clear");

  printBoard(board);
  printf("Final Points: %d\n", points);
  printf("Final Game Speed: %d\n", gameSpeed);

  tcsetattr(STDIN_FILENO, 0, &ATTR_ORIG);

  return 0;
}
