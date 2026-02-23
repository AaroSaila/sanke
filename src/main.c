#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "Board.h"
#include "BoardPiece.h"
#include "Snake.h"
#include "args.h"
#include "food.h"
#include "utils.h"

#define TERMIOS 1
#define DEBUG 0

#define MIN_SLEEP_TIME_MS 60

const char snake_vis = '#';

struct termios set_termios();

int main(int argc, char** argv) {
    Arguments args = cmd_args(argc, argv);

    srand(time(NULL));

    // Termios setup
#if TERMIOS
    const struct termios attr_orig = set_termios();
#endif // TERMIOS

    // Game model init
    const int board_w = args.width;
    const int board_h = args.height;
    Board board = board_alloc(board_w, board_h);
    Snake snake = snake_alloc(board_w * board_h, 0, 0, 'd');
    BoardPiece food = { .vis_char = '$' };
    food_new_location(&board, &food, &snake);
    unsigned int score = 0;
    unsigned int sleep_time = args.sleep_ms;

    // Screen init
    system("clear");

#if DEBUG
    long long frame = 0;
#endif // DEBUG

    while (true) {
        system("clear");

        // Process input
        char input = '0';
        read(STDIN_FILENO, &input, 1);

        if (isalpha(input)) {
            if (
                (input == 'w' && snake.dir != 's')
                || (input == 'a' && snake.dir != 'd')
                || (input == 's' && snake.dir != 'w')
                || (input == 'd' && snake.dir != 'a')) {
                snake_change_direction(&snake, input);
            }
        }

        snake_move(&snake, board.width, board.height);
        board_clear(&board);
        board_draw_snake(&board, &snake);
        board_set_square(&board, food.x, food.y, food.vis_char);
        print_board(&board);
        printf("Score: %d\n", score);

        BoardPiece snake_head = snake_get_part(&snake, 0);

        if (snake_collides_with_tail(&snake)) {
            printf("GAME OVER\n");
            break;
        }

        if (pieces_collide(&snake_head, &food)) {
            score++;
            snake_add_part(&snake);
            food_new_location(&board, &food, &snake);
            const unsigned int new_sleep_time = sleep_time - 5;
            if (new_sleep_time >= MIN_SLEEP_TIME_MS) {
                sleep_time = new_sleep_time;
            }
        }

#if DEBUG
        printf("input: %c\n", input);
        printf("Frame: %lld\n", frame);
        printf("sleep_ms: %u\n", sleep_time);
        snake_print_info(&snake);
        board_piece_print_info(&food, "Food");
        frame++;
#endif // DEBUG

        sleep_ms(sleep_time);
    }

#if TERMIOS
    tcsetattr(STDIN_FILENO, 0, &attr_orig);
#endif // TERMIOS

    board_free(&board);
    snake_free(&snake);

    return 0;
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
