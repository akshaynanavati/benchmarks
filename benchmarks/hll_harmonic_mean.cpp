#include <array>
#include <cmath>
#include <random>

#include <benchmark/benchmark.h>

long double harmonicMean1(const std::array<uint8_t, 256> &cont) {
  uint64_t nexp = 0;
  for (auto i : cont) {
    nexp += i;
  }

  long double den = 0;
  for (auto i : cont) {
    den += std::exp2(nexp - i);
  }

  long double z = std::exp2(nexp) / den;
  return z;
}

long double harmonicMean2(const std::array<uint8_t, 256> &cont) {
  long double z = 0;
  for (auto i : cont) {
    z += std::exp2(-i);
  }
  return 1 / z;
}

std::array<uint8_t, 256> generateArr() {
  static std::mt19937 rng;
  std::array<uint8_t, 256> arr;
  for (int i = 0; i < arr.size(); ++i) {
    arr[i] = rng() % 40;
  }
  return arr;
}

void BM_harmonicMean1(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto arr = generateArr();
    state.ResumeTiming();
    benchmark::DoNotOptimize(harmonicMean1(arr));
  }
}

void BM_harmonicMean2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto arr = generateArr();
    state.ResumeTiming();
    benchmark::DoNotOptimize(harmonicMean2(arr));
  }
}

BENCHMARK(BM_harmonicMean1);
BENCHMARK(BM_harmonicMean2);

BENCHMARK_MAIN();
