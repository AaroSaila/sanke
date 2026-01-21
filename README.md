# Sanke
A simple snake game in the terminal. No third party libraries needed.

## Demo
Click image for video

[![Sanke demo](https://img.youtube.com/vi/_coOVAW9qa4/0.jpg)](https://www.youtube.com/watch?v=_coOVAW9qa4)

## How it works
The program makes the terminal use non-canonical input and output by using termios. Then it updates and prints a 2D character array containing the board borders, snake, and food. Before terminating, the program restores canonical mode.

## Info
Only tested with the combination of
- GCC
- Linux
