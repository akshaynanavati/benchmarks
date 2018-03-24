#include <random>
#include <set>
#include <unordered_set>

#include <benchmark/benchmark.h>
#include <sparsehash/dense_hash_set>

#include "falcon/iterators/dense_set.h"

using namespace falcon;
typedef google::dense_hash_set<uint32_t, std::hash<uint32_t>> google_dense_set;

constexpr size_t kLookups = 1 << 15;
constexpr size_t kMinSize = 1 << 4;
constexpr size_t kMaxSize = 1 << 14;
constexpr uint32_t kRngClamp = (1 << 15) - 1;

uint32_t rng() {
  static std::mt19937 rng_;
  return rng_() & kRngClamp;
}

template <class Set> Set randomSet(size_t n) {
  Set set;
  for (size_t i = 0; i < n; ++i) {
    benchmark::DoNotOptimize(set.insert(rng()));
  }
  return set;
}

template <> google_dense_set randomSet<google_dense_set>(size_t n) {
  google_dense_set set;
  set.set_empty_key(std::numeric_limits<uint32_t>::max());
  for (size_t i = 0; i < n; ++i) {
    benchmark::DoNotOptimize(set.insert(rng()));
  }
  return set;
}

template <class Set> void BM_lookup(benchmark::State &state) {
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
  for (auto _ : state) {
    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(randomSet<Set>(state.range(0)));
    }
  }
}

BENCHMARK_TEMPLATE(BM_lookup, google_dense_set)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, DenseSet<uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, DenseSet<uint32_t, 2>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, std::set<uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, std::unordered_set<uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_TEMPLATE(BM_create, google_dense_set)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, DenseSet<uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, DenseSet<uint32_t, 2>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, std::set<uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, std::unordered_set<uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
