#include <random>

#include <benchmark/benchmark.h>

#include "deps/falcon/iterators/linked_list.h"

constexpr size_t kMinSize = 1 << 10;
constexpr size_t kMaxSize = 1 << 18;

using namespace falcon;

template <class T> struct TrivialLLNode {
  T data;
  std::unique_ptr<TrivialLLNode<T>> next = nullptr;

  TrivialLLNode(T &&data, std::unique_ptr<TrivialLLNode<T>> next = nullptr)
      : data(std::forward<T>(data)), next(std::move(next)) {}
};

template <class Node> std::unique_ptr<Node> random(size_t n) {
  std::mt19937_64 rng;
  auto head = std::make_unique<Node>(rng());
  while (--n) {
    head = std::make_unique<Node>(rng(), std::move(head));
  }
  return head;
}

void BM_falcon_destructor(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto head = random<LLNode<uint64_t>>(state.range(0));
    state.ResumeTiming();
    head.reset();
  }
}

void BM_trivial_destructor(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto head = random<TrivialLLNode<uint64_t>>(state.range(0));
    state.ResumeTiming();
    head.reset();
  }
}

BENCHMARK(BM_trivial_destructor)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);
BENCHMARK(BM_falcon_destructor)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
