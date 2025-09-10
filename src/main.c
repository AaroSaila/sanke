#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "Board.h"
#include "Snake.h"
#include "utils.h"
#include "food.h"
#include "BoardPiece.h"

#define TERMIOS 1
#define DEBUG 0

const char* VERSION = "1.1.9";

const char snake_vis = '#';

void cmd_args(int argc, char** argv);
void empty_stdin_buffer();
void get_int_or_minus_one(int* dst);
struct termios set_termios();

int main(int argc, char** argv) {
  cmd_args(argc, argv);

  srand(time(NULL));

  // Termios setup
#if TERMIOS
  const struct termios attr_orig = set_termios();
#endif // TERMIOS

  // Game model init
  const int board_w = 15;
  const int board_h = 15;
  Board board = board_alloc(board_w, board_h);
  Snake snake = snake_alloc(board_w * board_h, 0, 0, 'd');
  BoardPiece food = { .vis_char = '$' };
  food_new_location(&board, &food, &snake);
  unsigned int score = 0;

  // Screen init
  system("clear");

  int frame = 0;
  int fps = 0;

  long second_start = time(NULL);
  int frame_stamp = frame;

  while (true) {
    long second_check = time(NULL);
    long elapsed_time = second_check - second_start;
    if (elapsed_time >= 1) {
      fps = frame - frame_stamp;
      if (elapsed_time > 1) {
        fps /= 2;
      }
      frame_stamp = frame;
      second_start = time(NULL);
    }

    system("clear");

    // Process input
    char input = '0';
    read(STDIN_FILENO, &input, 1);

    if (isalpha(input)) {
      if (
          (input == 'w' && snake.dir != 's')
          || (input == 'a' && snake.dir != 'd')
          || (input == 's' && snake.dir != 'w')
          || (input == 'd' && snake.dir != 'a')
          ) {
        snake_change_direction(&snake, input);
      }
    }

    snake_move(&snake, board.width, board.height);   
    board_clear(&board);
    board_draw_snake(&board, &snake);
    board_set_square(&board, food.x, food.y, food.vis_char);
    printf("FPS: %d\n", fps);
    print_board(&board);
    printf("Score: %d\n", score);

    BoardPiece snake_head = snake_get_part(&snake, 0);

    if (snake_collides_with_tail(&snake)) {
      printf("GAME OVER\n");
      printf("Final score: %d\n", score);
      exit(EXIT_SUCCESS);
    }

    if (pieces_collide(&snake_head, &food)) {
      score++;
      snake_add_part(&snake);
      food_new_location(&board, &food, &snake);
    }

#if DEBUG
    printf("input: %c\n", input);
    printf("Frame: %d\n", frame);
    snake_print_info(&snake);
    board_piece_print_info(&food, "Food");
#endif // DEBUG

    frame++;
    sleep_ms(150);
  }

#if TERMIOS
  tcsetattr(STDIN_FILENO, 0, &attr_orig);
#endif // TERMIOS

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
