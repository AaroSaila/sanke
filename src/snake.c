#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "snake.h"

static SnakePart* snake_get_part_ptr(const Snake* snake, const size_t index) {
  if (index >= snake->max_length) {
    return NULL;
  }

  return snake->parts + index;
}

SnakePart snake_get_part(const Snake* snake, const size_t index) {
  assert(index < snake->max_length);
  return snake->parts[index];
}

Snake snake_alloc(
    const int board_square_count,
    const int init_x,
    const int init_y,
    const char init_dir
    )
{
  Snake snake = {0};
  snake.max_length = board_square_count;
  snake.length = 1;
  snake.dir = init_dir;
  snake.parts = (SnakePart*) malloc(sizeof(SnakePart) * snake.max_length);
  snake.parts[0] = (SnakePart) { .x = init_x, .y = init_y, .vis_char = '&' };

  return snake;
}

void snake_free(Snake* snake) {
  free(snake->parts);
}

void snake_move(Snake* snake) {
  SnakePart* first_part = snake_get_part_ptr(snake, 0);
  int first_part_old_x = first_part->x;
  int first_part_old_y = first_part->y;

  // Move first part
  switch (snake->dir) {
    case 'w':
      first_part->y -= 1;
      break;
    case 'a':
      first_part->x -= 1;
      break;
    case 's':
      first_part->y += 1;
      break;
    case 'd':
      first_part->x += 1;
      break;
    default:
      fprintf(stderr, "ERROR: Invalid direction in snake_move: %c.\n", snake->dir);
      exit(EXIT_FAILURE);
  }

  // Stop here if there is only one part
  SnakePart* last_part = snake_get_part_ptr(snake, snake->length - 1);
  if (last_part == first_part) {
    return;
  }

  // Move all other parts except for the second one, gets skipped if there are only 2 parts
  SnakePart* second_part = first_part + 1;
  SnakePart* prev_part = 0;
  for (SnakePart* part = last_part; part != second_part; part--) {
    prev_part = part - 1;
    part->x = prev_part->x;
    part->y = prev_part->y;
  }

  // Move second part
  second_part->x = first_part_old_x;
  second_part->y = first_part_old_y;
}
