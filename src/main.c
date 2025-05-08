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
#include "./board/board.h"

const char* VERSION = "1.1.9";

const char SNAKE_VIS = '#';

boardInfo brdInfo;
playableBoardInfo plBrdInfo;

void cmd_args(int argc, char** argv);
void emptyStdinBuffer();
void getIntOrMinusOne(int* dst);

int main(int argc, char** argv) {
  cmd_args(argc, argv);

  const clock_t initClock = clock();

  // Board Constraints
  printf("Set board size (15 - 60, default: 15): ");
  getIntOrMinusOne(&brdInfo.y);
  if (!(brdInfo.y >= 15 && brdInfo.y <= 60) || brdInfo.y == -1) {
    brdInfo.y = 15;
  }
  brdInfo.x = brdInfo.y * 2;
  /*printf("brdInfo.x: %d\n", brdInfo.x);*/
  /*printf("brdInfo.y: %d\n", brdInfo.y);*/
  char board[brdInfo.y][brdInfo.x];

  plBrdInfo.xs = 1;
  plBrdInfo.xe = brdInfo.x - 2;
  plBrdInfo.ys = 1;
  plBrdInfo.ye = brdInfo.y - 2;


  // Set gamespeed
  int gameSpeed = 0;
  printf("Enter gamespeed (default: 0, max: 180): ");
  getIntOrMinusOne(&gameSpeed);
  if (gameSpeed == -1 || gameSpeed > 180) {
    gameSpeed = 0;
  }


  // Termios setup
  struct termios attr;

  tcgetattr(STDIN_FILENO, &attr);

  const struct termios ATTR_ORIG = attr;

  attr.c_lflag &= ~(ECHO | ICANON);
  attr.c_cc[VMIN] = 0;
  attr.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, 0, &attr);

  // Game board setup
  setBoardBorders(board);

  int points = 0;
  const int sleepInterval = 200;

  // Snake head setup
  snakePart* snakeHead = malloc(sizeof(snakePart));
  if (!snakeHead) {
    mallocError("snakeHead", "main.c", "main()");
  }
  snakeHead->x = randomX(initClock);
  snakeHead->y = randomY(initClock);
  snakeHead->visChar = '&';
  snakeHead->dir = 'w';
  snakeHead->order = malloc(sizeof(order));
  if (!snakeHead->order) {
    mallocError("snakeHead->order", "main.c", "main()");
  }
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
      if (gameSpeed >= 100) {
        gameSpeed += 5;
      } else {
        gameSpeed += 10;
      }

      if (gameSpeed >= 180) {
        gameSpeed = 180;
      }

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

void cmd_args(int argc, char** argv) {
  if (argc == 1) {
    return;
  }

  if (
      strcmp(argv[1], "--version") == 0
      || strcmp(argv[1], "-v") == 0
      )
  {
    printf("Sanke version %s\n", VERSION);
    exit(0);
  }
}

void emptyStdinBuffer() {
  char ch;
  while ((ch = getchar()) != '\n');
}

void getIntOrMinusOne(int* dst) {
  char iBuf[100];
  char num[100];
  int i = 0;

  fgets(iBuf, 100, stdin);

  while (!isdigit(iBuf[i])) {
    if (iBuf[i] == '\0') {
      *dst = -1;
      return;
    }
    i++;
  }

  for (int j = 0; i < strlen(iBuf); i++, j++) {
    num[j] = iBuf[i];
  }

  *dst = atoi(num);
}
