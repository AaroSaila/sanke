#!/bin/sh

arg=-1

if [ -v 1 ]; then
  arg=$1
fi

if [ $arg = "clean" ]; then
  rm -rf ./target/*
  exit
fi

oflag="-Og"
debug_flag="-ggdb"

if [ $arg = "release" ]; then
  oflag="-O3"
  debug_flag=""
fi

src="src/*.c"
flags="-std=c23 $oflag $debug_flag -Wall -Wextra -Werror -Wpedantic -pedantic-errors"
includes="-I src/headers"

cmd="gcc $flags $includes $src -o target/sanke"

echo $cmd
$cmd


if [ $? -gt 0 ]; then
  exit
fi

if [ $arg = "run" ]; then
  echo 
  ./target/sanke
fi
