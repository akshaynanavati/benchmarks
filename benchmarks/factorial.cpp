#include <array>

#include <benchmark/benchmark.h>

#define BM_FACTORIAL(exec)                                                     \
  void BM_##exec(benchmark::State &state) {                                    \
    state.PauseTiming();                                                       \
    uint64_t n = 1ul << (state.range(0) + 20);                                 \
    state.ResumeTiming();                                                      \
    for (auto _ : state) {                                                     \
      benchmark::DoNotOptimize(factorial_##exec(n));                           \
    }                                                                          \
  }

uint64_t factorial_iterative(uint64_t n) {
  if (n == 0) {
    return 1;
  }

  uint64_t res = 1;

  for (; n > 0; --n) {
    res *= n;
  }
  return res;
}

uint64_t factorial_recursive(uint64_t n) {
  if (n == 0) {
    return 1;
  }

  return n * factorial_recursive(n - 1);
}

uint64_t factorial_tail(uint64_t n, uint64_t res = 1) {
  if (n == 0) {
    return res;
  }
  return factorial_tail(n - 1, res * n);
}

BM_FACTORIAL(recursive);
BM_FACTORIAL(iterative);
BM_FACTORIAL(tail);

static void args(benchmark::internal::Benchmark *b) {
  for (int i = 0; i < 18; ++i) {
    b->Args({i});
  }
}

BENCHMARK(BM_iterative)->Apply(args);
BENCHMARK(BM_tail)->Apply(args);
BENCHMARK(BM_recursive)->Apply(args);

BENCHMARK_MAIN();
