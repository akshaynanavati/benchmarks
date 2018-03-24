#include <map>
#include <random>

#include <benchmark/benchmark.h>

#include "falcon/trees/avl.h"
#include "falcon/trees/redblack.h"
#include "falcon/trees/splay.h"

using namespace falcon;

constexpr size_t kLookups = 1 << 15;
constexpr size_t kMinSize = 1 << 4;
constexpr size_t kMaxSize = 1 << 14;
constexpr uint32_t kRngClamp = (1 << 15) - 1;

uint32_t rng() {
  static std::mt19937 rng_;
  return rng_() & kRngClamp;
}

template <class Map> Map randomMap(size_t n) {
  Map map;
  for (size_t i = 0; i < n; ++i) {
    map.emplace(rng(), rng());
    benchmark::ClobberMemory();
  }
  return map;
}

template <class Map> void BM_lookup(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto map = randomMap<Map>(state.range(0));
    state.ResumeTiming();

    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(map.find(rng()));
    }
  }
}

template <class Map> void BM_create(benchmark::State &state) {
  for (auto _ : state) {
    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(randomMap<Map>(state.range(0)));
    }
  }
}

BENCHMARK_TEMPLATE(BM_lookup, AVLTree<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, RBTree<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, SplayTree<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_lookup, std::map<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_TEMPLATE(BM_create, AVLTree<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, RBTree<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, SplayTree<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_create, std::map<uint32_t, uint32_t>)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
