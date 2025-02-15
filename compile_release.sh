#!/bin/bash

gcc -Wall -Werror -pedantic -O3 -o sanke src/main.c src/snake/**.c src/utils/**.c src/board/**.c;
