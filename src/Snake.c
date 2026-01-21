#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Snake.h"
#include "utils.h"

extern const char snake_vis;

static BoardPiece* snake_get_part_ptr(const Snake* snake, const size_t index) {
  if (index >= snake->max_length) {
    return NULL;
  }

  return snake->parts + index;
}

BoardPiece snake_get_part(const Snake* snake, const size_t index) {
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
  snake.parts = (BoardPiece*) malloc(sizeof(BoardPiece) * snake.max_length);
  if (snake.parts == NULL) {
      mallocError("snake.parts", __FILE__, "snake_alloc");
  }
  snake.parts[0] = (BoardPiece) { .x = init_x, .y = init_y, .vis_char = '&' };

  return snake;
}

void snake_free(Snake* snake) {
  free(snake->parts);
}

static void check_bounds(Snake* snake, const int width, const int height) {
  for (size_t i = 0;i < snake->length; i++) {
    BoardPiece* part = snake->parts + i;
    if (part->x < 0) {
      part->x = width - 2;
    } else if (part->x >= width) {
      part->x = 0;
    }

    if (part->y < 0) {
      part->y = height - 1;
    } else if (part->y >= height) {
      part->y = 0;
    }
  }
}

void snake_move(Snake* snake, const int width, const int height) {
  BoardPiece* first_part = snake_get_part_ptr(snake, 0);
  int first_part_old_x = first_part->x;
  int first_part_old_y = first_part->y;

  // Move first part
  switch (snake->dir) {
    case 'w':
      first_part->y -= 1;
      break;
    case 'a':
      first_part->x -= 2;
      break;
    case 's':
      first_part->y += 1;
      break;
    case 'd':
      first_part->x += 2;
      break;
    default:
      fprintf(stderr, "ERROR: Invalid direction in snake_move: %c.\n", snake->dir);
      exit(EXIT_FAILURE);
  }

  // Stop here if there is only one part
  BoardPiece* last_part = snake_get_part_ptr(snake, snake->length - 1);
  if (last_part == first_part) {
    check_bounds(snake, width, height);
    return;
  }

  // Move all other parts except for the second one, gets skipped if there are only 2 parts
  BoardPiece* second_part = first_part + 1;
  BoardPiece* prev_part = 0;
  for (BoardPiece* part = last_part; part != second_part; part--) {
    prev_part = part - 1;
    part->x = prev_part->x;
    part->y = prev_part->y;
  }

  // Move second part
  second_part->x = first_part_old_x;
  second_part->y = first_part_old_y;

  check_bounds(snake, width, height);
}

void snake_print_info(Snake* snake) {
  printf("snake: {\n");
  printf("  parts: {\n");
  for (size_t i = 0; i < snake->length; i++) {
    BoardPiece part = snake->parts[i];
    printf("    x: %d\n", part.x);
    printf("    y: %d\n", part.y);
  }
  printf("  }\n");

  printf("  max_length: %zu\n", snake->max_length);
  printf("  length: %zu\n", snake->length);
  printf("  dir: %c\n", snake->dir);
  printf("}\n");
}

void snake_change_direction(Snake* snake, const char direction) {
  snake->dir = direction;
}

bool snake_collides(const Snake* snake, const BoardPiece* piece) {
  for (size_t i = 0; i < snake->length; i++) {
    BoardPiece part = snake->parts[i];
    if (part.x == piece->x && part.y == piece->y) {
      return true;
    }
  }

  return false;
}

bool snake_collides_with_tail(const Snake* snake) {
  const BoardPiece* head = snake->parts;
  for (size_t i = 1; i < snake->length; i++) {
    BoardPiece part = snake->parts[i];
    if (part.x == head->x && part.y == head->y) {
      return true;
    }
  }

  return false;
}

void snake_add_part(Snake* snake) {
  if (snake->length == snake->max_length) {
    fprintf(stderr, "ERROR: Cannot add another part to snake. Would exceed max_length.\n");
    exit(EXIT_FAILURE);
  }

  int x_shift;
  int y_shift;
  BoardPiece last_part;
  char prev_part_dir;

  if (snake->length == 1) {
    last_part = snake_get_part(snake, 0);
    prev_part_dir = snake->dir;
  } else {
    last_part = snake_get_part(snake, snake->length - 1);
    const BoardPiece second_to_last_part = snake_get_part(snake, snake->length - 2);
    
    if (second_to_last_part.y < last_part.y && second_to_last_part.x == last_part.x) {
      prev_part_dir = 'w';
    } else if (second_to_last_part.x < last_part.x && second_to_last_part.y == last_part.y) {
      prev_part_dir = 'a';
    } else if (second_to_last_part.y > last_part.y && second_to_last_part.x == last_part.x) {
      prev_part_dir = 's';
    } else if (second_to_last_part.x > last_part.x && second_to_last_part.y == last_part.y) {
      prev_part_dir = 'd';
    } else {
      fprintf(stderr, "%s:%d: ERROR: Invalid direction.\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }
  }

  switch (prev_part_dir) {
    case 'w':
      x_shift = 0;
      y_shift = 1;
      break;
    case 'a':
      x_shift = 1;
      y_shift = 0;
      break;
    case 's':
      x_shift = 0;
      y_shift = -1;
      break;
    case 'd':
      x_shift = -1;
      y_shift = 0;
      break;
    default:
      fprintf(stderr, "%s:%d: ERROR: Invalid direction.\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
  }

  snake->parts[snake->length] = (BoardPiece){
    .x = last_part.x + x_shift,
    .y = last_part.y + y_shift,
    .vis_char = snake_vis
  };
  snake->length++;
}
