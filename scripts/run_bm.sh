#!/bin/sh

set -e

g++-7 -std=c++11 -O3 -lbenchmark benchmarks/$1.cpp
./a.out --benchmark_format=csv > results/$1_results.csv
./scripts/normalize_csv.py results/$1_results.csv
