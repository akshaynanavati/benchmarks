#include <random>
#include <set>
#include <unordered_set>

#include <benchmark/benchmark.h>

#include "deps/falcon/iterators/dense_set.h"

using namespace falcon;

constexpr size_t kLookups = 1 << 15;
constexpr size_t kMinSize = 1 << 5;
constexpr size_t kMaxSize = 1 << 15;

template <class Set> Set randomSet(size_t n) {
  static std::mt19937_64 rng;
  Set set;
  for (size_t i = 0; i < n; ++i) {
    benchmark::DoNotOptimize(set.insert(rng()));
  }
  return set;
}

template <class Set> void BM_lookup(benchmark::State &state) {
  static std::mt19937_64 rng;
  for (auto _ : state) {
    state.PauseTiming();
    auto set = randomSet<Set>(state.range(0));
    state.ResumeTiming();

    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(set.find(rng()));
    }
  }
}

template <class Set> void BM_create(benchmark::State &state) {
  static std::mt19937_64 rng;
  for (auto _ : state) {
    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(randomSet<Set>(state.range(0)));
    }
  }
}

BENCHMARK_TEMPLATE(BM_lookup, DenseSet<uint64_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, DenseSet<uint64_t, 2>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, std::set<uint64_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, std::unordered_set<uint64_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_TEMPLATE(BM_create, DenseSet<uint64_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, DenseSet<uint64_t, 2>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, std::set<uint64_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, std::unordered_set<uint64_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
