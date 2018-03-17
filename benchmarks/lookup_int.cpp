#include <algorithm>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include <benchmark/benchmark.h>

constexpr size_t kLookups = 1 << 15;
constexpr size_t kMinSize = 1 << 5;
constexpr size_t kMaxSize = 1 << 15;

template <class Int> void BM_vector(benchmark::State &state) {
  std::mt19937_64 rng;
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<Int> vec(state.range(0), 0);
    std::generate(vec.begin(), vec.end(), rng);
    state.ResumeTiming();
    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(std::find(vec.begin(), vec.end(), rng()));
    }
  }
}

template <template <typename> class Set, class Int>
void BM_set(benchmark::State &state) {
  std::mt19937_64 rng;
  for (auto _ : state) {
    state.PauseTiming();
    Set<Int> s;
    for (size_t i = 0; i < kLookups; ++i) {
      s.insert(rng());
    }
    state.ResumeTiming();
    for (int i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(s.find(rng()));
    }
  }
}

BENCHMARK_TEMPLATE(BM_vector, uint64_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_set, std::set, uint64_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_set, std::unordered_set, uint64_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_vector, uint8_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_set, std::set, uint8_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_set, std::unordered_set, uint8_t)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
