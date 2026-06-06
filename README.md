# Sanke
A simple snake game in the terminal. No third party libraries needed.

# Compiling
Run:
```sh
compile.sh release
```
to compile an optimized build. The script creates a directory called 'target' that contains the executable.

See:
```sh
compile.sh help
```
for more.

# Running
Run:
```sh
sanke 
```
to play with defaults settings.

See:
```sh
sanke --help
```
for setting game options.

# How it works
The program makes the terminal use non-canonical input and output by using termios. Then it updates and prints a 2D character array containing the board borders, snake, and food. Before terminating, the program restores canonical mode.

# Info
Only tested with
- Clang
- GCC
- Linux
