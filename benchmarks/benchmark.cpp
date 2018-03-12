#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>

#define BENCHMARK(cls)                                                         \
  {                                                                            \
    Benchmark<cls> b;                                                          \
    std::cout << "class," << #cls << "-insert" << std::endl;                   \
    b.insertions();                                                            \
    b.reset();                                                                 \
    std::cout << "class," << #cls << "-lookup" << std::endl;                   \
    b.lookups();                                                               \
  }

constexpr auto kIterations = 1 << 8;
constexpr auto kMinSize = 1 << 5;
constexpr auto kMaxSize = 1 << 15;
constexpr auto kOperations = 1 << 7;

class UnsortedVectorBench {
  std::vector<uint64_t> vec_;

public:
  void insert(uint64_t i) { vec_.push_back(i); }

  std::vector<uint64_t>::iterator find(uint64_t i) {
    return std::find(vec_.begin(), vec_.end(), i);
  }

  std::vector<uint64_t>::iterator end() { return vec_.end(); }
  std::vector<uint64_t>::iterator begin() { return vec_.begin(); }
};

template <class Container> class Benchmark {
  std::mt19937 rng_;
  Container container_;

  void fill(size_t size) {
    for (size_t j = 0; j < size; ++j) {
      container_.insert(rng_());
    }
  }

public:
  void reset() { container_ = Container(); }

  void insertions() {
    std::cout << "size,time" << std::endl;
    for (size_t size = kMinSize; size <= kMaxSize; size <<= 1) {
      auto start = std::clock();
      for (size_t i = 0; i < kIterations; ++i) {
        fill(size);
        reset();
      }
      auto end = std::clock();
      std::cout << size << "," << (end - start) / (double)kIterations
                << std::endl;
      // Force compiler to not optimize this out
      uint64_t init = 0;
      std::cerr << std::accumulate(container_.begin(), container_.end(), init)
                << std::endl;
    }
  }

  void deletions() {}

  void lookups() {
    std::cout << "size,time" << std::endl;
    for (size_t size = kMinSize; size <= kMaxSize; size <<= 1) {
      fill(size);
      auto start = std::clock();
      for (size_t i = 0; i < kIterations; ++i) {
        for (size_t j = 0; j < size; ++j) {
          // Force compiler to not optimize this out
          std::cerr << (container_.find(rng_()) != container_.end()
                            ? "FOUND!"
                            : "NOT FOUND :(")
                    << std::endl;
        }
      }
      auto end = std::clock();
      reset();
      std::cout << size << "," << (end - start) / (double)kIterations
                << std::endl;
    }
  }

  // TODO only works for vector
  void sortlookup() {
    std::cout << "size,time" << std::endl;
    for (size_t size = kMinSize; size <= kMaxSize; size <<= 1) {
      fill(size);
      std::sort(container_.begin(), container_.end());
      auto start = std::clock();
      for (size_t i = 0; i < kIterations; ++i) {
        for (size_t j = 0; j < size; ++j) {
          // Force compiler to not optimize this out
          std::cerr << (std::binary_search(container_.begin(), container_.end(),
                                           rng_())
                            ? "FOUND!"
                            : "NOT FOUND :(")
                    << std::endl;
        }
      }
      auto end = std::clock();
      reset();
      std::cout << size << "," << (end - start) / (double)kIterations
                << std::endl;
    }
  }

  void randomOps() {}
};

int main() {
  Benchmark<UnsortedVectorBench> b;
  b.sortlookup();
}
