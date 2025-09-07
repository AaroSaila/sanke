#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "snake.h"
#include "utils.h"

const char* VERSION = "1.1.9";

const char SNAKE_VIS = '#';

void cmd_args(int argc, char** argv);
void empty_stdin_buffer();
void get_int_or_minus_one(int* dst);
struct termios set_termios();

int main() {
  // cmd_args(argc, argv);

  // const clock_t initClock = clock();

  // Set gamespeed
  // int gameSpeed = 0;
  // printf("Enter gamespeed (default: 0, max: 180): ");
  // getIntOrMinusOne(&gameSpeed);
  // if (gameSpeed == -1 || gameSpeed > 180) {
  //   gameSpeed = 0;
  // }


  // Termios setup
  const struct termios attr_orig = set_termios();

  // Game model init
  const int board_w = 15;
  const int board_h = 15;
  Board board = board_alloc(board_w, board_h);
  Snake snake = snake_alloc(board_w * board_h, 0, 0, 'd');

  // Screen init
  system("clear");

  while (true) {
    system("clear");
    snake_move(&snake);   
    board_clear(&board);
    board_draw_snake(&board, &snake);
    print_board(&board);
    sleep_ms(1000);
  }

  tcsetattr(STDIN_FILENO, 0, &attr_orig);

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

void empty_stdin_buffer() {
  char ch;
  while ((ch = getchar()) != '\n');
}

void get_int_or_minus_one(int* dst) {
  char iBuf[100];
  char num[100];
  size_t i = 0;

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

struct termios set_termios() {
  struct termios attr;

  tcgetattr(STDIN_FILENO, &attr);

  const struct termios attr_orig = attr;

  attr.c_lflag &= ~(ECHO | ICANON);
  attr.c_cc[VMIN] = 0;
  attr.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, 0, &attr);

  return attr_orig;
}
