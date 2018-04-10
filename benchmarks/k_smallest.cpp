#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#define BM_SELECT(f)                                                           \
  void BM_##f(benchmark::State &state) {                                       \
    for (auto _ : state) {                                                     \
      state.PauseTiming();                                                     \
      auto vec = randomVec(n);                                                 \
      state.ResumeTiming();                                                    \
      benchmark::DoNotOptimize(f(vec, state.range(0)));                        \
      benchmark::ClobberMemory();                                              \
    }                                                                          \
  }                                                                            \
  BENCHMARK(BM_##f)->RangeMultiplier(2)->Range(minK, maxK);

typedef std::vector<uint64_t> Vector;

constexpr size_t n = 1 << 23;
constexpr size_t minK = 1 << 5;
constexpr size_t maxK = 1 << 15;

Vector randomVec(size_t n) {
  static std::mt19937_64 rng(time(NULL));
  Vector res(n);
  std::generate(res.begin(), res.end(), rng);
  return res;
}

Vector quickSelect(Vector &vec, size_t k) {
  std::nth_element(vec.begin(), vec.begin() + k - 1, vec.end());
  Vector res(k);
  std::copy(vec.begin(), vec.begin() + k, res.begin());
  return res;
}

Vector sortSelect(Vector &vec, size_t k) {
  std::sort(vec.begin(), vec.end());
  Vector res(k);
  std::copy(vec.begin(), vec.begin() + k, res.begin());
  return res;
}

Vector heapSelect(Vector &vec, size_t k) {
  std::make_heap(vec.begin(), vec.begin() + k);
  for (auto it = vec.begin() + k; it < vec.end(); ++it) {
    if (*it <= vec[0]) {
      std::pop_heap(vec.begin(), vec.end());
      vec[k - 1] = *it;
      std::push_heap(vec.begin(), vec.end());
    }
  }
  Vector res(k);
  std::copy(vec.begin(), vec.begin() + k, res.begin());
  return res;
}

BM_SELECT(heapSelect);
BM_SELECT(quickSelect);
BM_SELECT(sortSelect);

BENCHMARK_MAIN();
