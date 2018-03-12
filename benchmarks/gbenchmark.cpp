#include "benchmark/benchmark.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>

constexpr auto kIterations = 1 << 8;
constexpr auto kMinSize = 1 << 5;
constexpr auto kMaxSize = 1 << 15;
constexpr auto kOperations = 1 << 10;

class UnsortedVectorBench {
  std::vector<uint64_t> vec_;

public:
  std::vector<uint64_t>::reference insert(uint64_t i) {
    vec_.push_back(i);
    return vec_.back();
  }

  std::vector<uint64_t>::iterator find(uint64_t i) {
    return std::find(vec_.begin(), vec_.end(), i);
  }

  std::vector<uint64_t>::iterator end() { return vec_.end(); }
  std::vector<uint64_t>::iterator begin() { return vec_.begin(); }
};

template <class Container> class Benchmark {
  static void fill(Container &container, size_t size) {
    std::mt19937_64 rng;
    for (size_t j = 0; j < size; ++j) {
      container.insert(rng());
    }
  }

public:
  static void BM_insert(benchmark::State &state) {
    Container data;
    std::mt19937_64 rng;
    for (auto _ : state) {
      state.PauseTiming();
      // Generate random numbers
      std::vector<uint64_t> rand(state.range(0));
      std::generate(rand.begin(), rand.end(), rng);
      state.ResumeTiming();
      for (int j = 0; j < state.range(0); ++j) {
        benchmark::DoNotOptimize(data.insert(rand[j]));
      }
    }
  }

  static void BM_lookup(benchmark::State &state) {
    Container data;
    std::mt19937_64 rng;
    for (auto _ : state) {
      state.PauseTiming();
      fill(data, state.range(0));
      // Generate random numbers
      std::vector<uint64_t> rand(state.range(1));
      std::generate(rand.begin(), rand.end(), rng);
      state.ResumeTiming();
      for (int j = 0; j < state.range(1); ++j) {
        benchmark::DoNotOptimize(data.find(rand[j]));
      }
    }
  }
};

const auto BM_set_insert = Benchmark<std::set<uint64_t>>::BM_insert;
const auto BM_set_lookup = Benchmark<std::set<uint64_t>>::BM_lookup;
const auto BM_uset_insert = Benchmark<std::unordered_set<uint64_t>>::BM_insert;
const auto BM_uset_lookup = Benchmark<std::unordered_set<uint64_t>>::BM_lookup;
const auto BM_vector_insert = Benchmark<UnsortedVectorBench>::BM_insert;
const auto BM_vector_lookup = Benchmark<UnsortedVectorBench>::BM_lookup;

BENCHMARK(BM_set_insert)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);
BENCHMARK(BM_uset_insert)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);
BENCHMARK(BM_vector_insert)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);

BENCHMARK(BM_set_lookup)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {kOperations, kOperations}});
BENCHMARK(BM_uset_lookup)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {kOperations, kOperations}});
BENCHMARK(BM_vector_lookup)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {kOperations, kOperations}});

BENCHMARK_MAIN();
