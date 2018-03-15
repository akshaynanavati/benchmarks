#include <algorithm>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include <benchmark/benchmark.h>

constexpr size_t kLookups = 1 << 15;
constexpr size_t kMinSize = 1 << 5;
constexpr size_t kMaxSize = 1 << 15;
constexpr size_t kStringSize = 31;

std::string randString() {
  static std::mt19937 rng32;
  auto rngc = []() { return (char)(rng32() & 127); };
  std::string s(kStringSize, 'a');
  std::generate(s.begin(), s.end(), rngc);
  return s;
}

void BM_vector(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<std::string> vec(state.range(0));
    std::generate(vec.begin(), vec.end(), randString);
    state.ResumeTiming();
    for (size_t i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(std::find(vec.begin(), vec.end(), randString()));
    }
  }
}

template <template <typename> class Set> void BM_set(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Set<std::string> s;
    for (size_t i = 0; i < kLookups; ++i) {
      s.insert(randString());
    }
    state.ResumeTiming();
    for (int i = 0; i < kLookups; ++i) {
      benchmark::DoNotOptimize(s.find(randString()));
    }
  }
}

BENCHMARK(BM_vector)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_set, std::set)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);
BENCHMARK_TEMPLATE(BM_set, std::unordered_set)
    ->RangeMultiplier(2)
    ->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
