#include <stdlib.h>

#include "food.h"
#include "Board.h"
#include "BoardPiece.h"
#include "Snake.h"

void food_new_location(Board* board, BoardPiece* food, Snake* snake) {
  do {
    food->x = rand() % board->width;
    food->y = rand() % board->height;
  } while (food->x % 2 != 0 || snake_collides(snake, food));
}
