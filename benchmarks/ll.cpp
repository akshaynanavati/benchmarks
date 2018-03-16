#include <memory>
#include <random>

#include <benchmark/benchmark.h>

#define BM_SUM(exec)                                                           \
  void BM_sum_##exec(benchmark::State &state) {                                \
    state.PauseTiming();                                                       \
    auto head = randomLL(state.range(0));                                      \
    state.ResumeTiming();                                                      \
    for (auto _ : state) {                                                     \
      benchmark::DoNotOptimize(ll_sum_##exec(head.get()));                     \
    }                                                                          \
  }

constexpr size_t kMinSize = 1 << 5;
constexpr size_t kMaxSize = 1 << 15;

template <class T> struct LLNode {
  T data;
  std::unique_ptr<LLNode<T>> next = nullptr;

  LLNode(T &&data, std::unique_ptr<LLNode<T>> next = nullptr)
      : data(std::forward<T>(data)), next(std::move(next)) {}
};

std::unique_ptr<LLNode<uint64_t>> randomLL(size_t n) {
  std::mt19937_64 rng;
  auto head = std::make_unique<LLNode<uint64_t>>(rng());
  while (--n) {
    head = std::make_unique<LLNode<uint64_t>>(rng(), std::move(head));
  }
  return head;
}

uint64_t ll_sum_recursive(LLNode<uint64_t> *head) {
  if (head == nullptr) {
    return 0;
  }

  return head->data + ll_sum_recursive(head->next.get());
}

uint64_t ll_sum_tail(LLNode<uint64_t> *head, uint64_t sum = 0) {
  if (head == nullptr) {
    return sum;
  }

  return ll_sum_tail(head->next.get(), head->data + sum);
}

uint64_t ll_sum_iterative(LLNode<uint64_t> *head) {
  uint64_t sum = 0;
  while (head) {
    sum += head->data;
    head = head->next.get();
  }
  return sum;
}

BM_SUM(iterative);
BM_SUM(tail);
BM_SUM(recursive);

BENCHMARK(BM_sum_iterative)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);
BENCHMARK(BM_sum_tail)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);
BENCHMARK(BM_sum_recursive)->RangeMultiplier(2)->Range(kMinSize, kMaxSize);

BENCHMARK_MAIN();
