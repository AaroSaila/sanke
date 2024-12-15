# Sanke
A simple snake game in the terminal. No third party libraries needed.

## Demo
Click image for video

[![Sanke demo](https://img.youtube.com/vi/_coOVAW9qa4/0.jpg)](https://www.youtube.com/watch?v=_coOVAW9qa4)

## How it works
The program makes the terminal use non-canonical input and output by using termios. Then it updates and prints a 2D character array containing the board borders, snake, and food. Before terminating, the program restores canonical mode.


Each part of the snake is a node of a dynamically allocated linked list that contains:
- The snake's x and y coordinates
- The character that represents it on the board
- The part's direction
- A pointer to a order node (more on orders below)
- A pointer to the next snake part node

An order keeps information on when the direction of the snake part has to change and to which direction it should change. An order is also a node in a dynamically allocated linked list that contains:
- The direction that this order eventually tells the snake part to go to
- The delay i.e. how many ticks until the snake part has to change direction.
- A pointer to the next order

## Info
Only tested and designed to work with the combination of
- GCC
- Linux
- x86 CPU
