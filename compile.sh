#!/bin/sh

src="src/*.c"
flags="-std=c23 -ggdb -Og -Wall -Wextra -Werror -Wpedantic -pedantic-errors"
includes="-I src/headers"

cmd="gcc $flags $includes $src -o target/sanke"

echo $cmd
$cmd

if [ $? -gt 0 ]; then
  exit
fi

echo 

if [ -v 1 ]; then
  if [ $1 = "run" ]; then
    ./target/sanke
  fi
fi
