#include <ctime>
#include <random>
#include <string>

#include <benchmark/benchmark.h>

#include "falcon/sets/hll.h"

using namespace falcon;

// Generate random numbers and feed them to the HLL algorithm. Each
// call should generate a unique "hash" and we can see how close the
// count is to actual number of calls.
template <class Key> class Hash {
  std::mt19937_64 rng_;

public:
  Hash() { rng_.seed(std::time(NULL)); }

  uint64_t operator()(const Key & /* unused */) { return rng_(); }
};

void BM_hll(benchmark::State &state) {
  for (auto _ : state) {
    HyperLogLog<std::string, Hash> hll;
    size_t n = 1 << state.range(0);
    for (size_t i = 0; i < n; ++i) {
      hll.insert("hello");
    }
    benchmark::DoNotOptimize(hll.count());
  }
}

BENCHMARK(BM_hll)->DenseRange(10, 30);
BENCHMARK_MAIN();
