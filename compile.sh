#!/bin/bash

gcc -Og -g -Wall -Werror -pedantic -o build/sanke src/main.c src/snake/**.c src/utils/**.c src/board/**.c
