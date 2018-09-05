#include <algorithm>
#include <ctime>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include <benchmark/benchmark.h>

constexpr size_t kLookups = 1 << 15;
constexpr size_t kMinSize = 1 << 5;
constexpr size_t kMaxSize = 1 << 15;
constexpr size_t kStringSize = 64;

static std::mt19937 rng32{(uint32_t)time(NULL)};

std::string randString() {
  auto rngc = []() { return (char)(rng32() % 127 + 1); };
  std::string s(kStringSize, 'a');
  std::generate(s.begin(), s.end(), rngc);
  return s;
}

std::vector<std::string> getKeysToLookup() {
  std::vector<std::string> toLookup{kLookups};
  std::generate(toLookup.begin(), toLookup.end(), randString);
  return toLookup;
}

void BM_vector(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<std::string> vec(state.range(0));
    std::generate(vec.begin(), vec.end(), randString);
    auto toLookup = getKeysToLookup();
    state.ResumeTiming();
    for (const auto &key : toLookup) {
      benchmark::DoNotOptimize(std::find(vec.begin(), vec.end(), key));
    }
  }
}

void BM_vector_bin_search(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<std::string> vec(state.range(0));
    std::generate(vec.begin(), vec.end(), randString);
    std::sort(vec.begin(), vec.end());
    auto toLookup = getKeysToLookup();
    state.ResumeTiming();
    for (const auto &key : toLookup) {
      benchmark::DoNotOptimize(std::binary_search(vec.begin(), vec.end(), key));
    }
  }
}

template <template <typename> class Set> void BM_set(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Set<std::string> s;
    for (auto i = 0; i < state.range(0); ++i) {
      s.insert(randString());
    }
    auto toLookup = getKeysToLookup();
    state.ResumeTiming();
    for (const auto &key : toLookup) {
      benchmark::DoNotOptimize(s.find(key));
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
BENCHMARK(BM_vector_bin_search)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
