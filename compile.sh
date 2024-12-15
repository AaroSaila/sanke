#!/bin/bash

gcc -Wall -Werror -pedantic -o sanke src/main.c src/snake/**.c src/utils/**.c
