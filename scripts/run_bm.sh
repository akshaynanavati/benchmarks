#!/bin/sh

set -e

if [ "$2" == "clang" ]
then
COMPILER='/usr/local/opt/llvm/bin/clang++'
EXTRA_FLAGS="-L$HOME/clang-lib/ -lbenchmark -I$HOME/clang-lib/"
OUT_FILE="results/$1_results_clang.csv"
BIN_NAME="$1_clang.out"
else
COMPILER='/usr/local/bin/g++-7'
EXTRA_FLAGS='-lbenchmark'
OUT_FILE="results/$1_results_gcc.csv"
BIN_NAME="$1_gcc.out"
fi

echo "Compilng with $COMPILER..."

$COMPILER $EXTRA_FLAGS -std=c++17 -O3 -o$BIN_NAME benchmarks/$1.cpp
./$BIN_NAME --benchmark_format=csv > $OUT_FILE
./scripts/normalize_csv.py $OUT_FILE
