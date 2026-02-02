#!/bin/sh

arg=-1

if [ -v 1 ]; then
  arg=$1
fi

if [ $arg = "help" ]; then
    echo "USAGE: compile.sh <option>"
    echo ""
    echo "Compiles debug build when called without an option."
    echo "Options:"
    echo "  help   : Show this help text."
    echo "  clean  : Clean target directory."
    echo "  release: Build with optimizations."
    echo "  run    : Build debug build and run."
    exit
fi

if [ $arg = "clean" ]; then
  rm -rf ./target/*
  exit
fi

version=$(git rev-parse HEAD)

oflag="-Og"
debug_flag="-ggdb"

if [ $arg = "release" ]; then
  oflag="-O3"
  debug_flag=""
fi

src="src/*.c"
macros="-DVERSION=\"$version\""
flags="-std=c23 $oflag $debug_flag $macros -Wall -Wextra -Werror -Wpedantic -pedantic-errors"
includes="-I src/headers"

cmd="gcc $flags $includes $src -o target/sanke"

echo $cmd
$cmd


if [ $? -gt 0 ]; then
  exit
fi

if [ $arg = "run" ]; then
  ./target/sanke
fi
