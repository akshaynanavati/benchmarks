#include <algorithm>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

constexpr size_t kMinSize = 1 << 5;
constexpr size_t kMaxSize = 1 << 20;

template <class Int> std::vector<Int> randomVector(size_t n) {
  static std::mt19937_64 rng;
  std::vector<Int> vec(n, 0);
  std::generate(vec.begin(), vec.end(), rng);
  return vec;
}

template <class Int> void BM_accum(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto vec = randomVector<Int>(state.range(0));
    state.ResumeTiming();
    benchmark::DoNotOptimize(std::accumulate(vec.begin(), vec.end(), 0));
  }
}

template <class Int> void BM_for(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto vec = randomVector<Int>(state.range(0));
    const size_t n = vec.size();
    state.ResumeTiming();
    Int res = 0;
    for (size_t i = 0; i < n; ++i) {
      res += vec[i];
    }
    benchmark::DoNotOptimize(res);
  }
}

template <class Int> void BM_range_for(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto vec = randomVector<Int>(state.range(0));
    state.ResumeTiming();
    Int res = 0;
    for (auto i : vec) {
      res += i;
    }
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK_TEMPLATE(BM_accum, uint64_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_for, uint64_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_range_for, uint64_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_TEMPLATE(BM_accum, uint8_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_for, uint8_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_range_for, uint8_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
