#!/bin/sh

set -e

OUT_FILE="results/$1_results.csv"

buck build benchmarks/:$1
./buck-out/gen/benchmarks/$1 --benchmark_format=csv > $OUT_FILE
./scripts/normalize_csv.py $OUT_FILE
