#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <benchmark/benchmark.h>

constexpr size_t nChars = 62;
constexpr std::array<char, nChars> kChars = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

constexpr size_t kMinSize = 1 << 6;
constexpr size_t kMaxSize = 1 << 15;

class StringSet {
  const std::string s_;

public:
  template <class String>
  explicit StringSet(String &&s) : s_(std::forward<String>(s)) {}

  std::string::const_iterator find(char c) const {
    return std::find(s_.begin(), s_.end(), c);
  }

  size_t size() const { return s_.size(); }

  std::string::const_iterator end() const { return s_.end(); }
};

template <bool sort>
std::pair<std::string, StringSet> getTestCaseSS(size_t n, size_t m) {
  std::mt19937 rng;

  std::string s;
  std::string ss;
  for (int i = 0; i < n; ++i) {
    s.push_back(kChars[rng() % nChars]);
  }

  for (int i = 0; i < m; ++i) {
    ss.push_back(s[rng() % n]);
  }

#if sort
  std::sort(s.begin(), s.end());
#endif

  return {std::move(s), StringSet(std::move(ss))};
}

template <class Set>
std::pair<std::string, Set> getTestCaseS(size_t n, size_t m) {
  std::mt19937 rng;

  std::string s;
  Set ss;
  for (int i = 0; i < n; ++i) {
    s.push_back(kChars[rng() % nChars]);
  }

  for (int i = 0; i < m; ++i) {
    ss.insert(s[rng() % n]);
  }

  return {std::move(s), std::move(ss)};
}

template <class Map, class Set>
std::string find_smallest_substring_with(const std::string &s,
                                         const Set &chars) {
  const size_t nchars = chars.size();
  const size_t n = s.size();
  Map indices;
  size_t minSize = n;
  size_t minLow = 0;

  size_t low = 0;
  size_t high = 0;

  while (1) {
    for (; indices.size() < nchars && high < n; ++high) {
      const auto c = s[high];
      if (chars.find(c) != chars.end()) {
        ++indices[c];
      }
    }

    if (high == n) {
      break;
    }

    for (; indices.size() == nchars; ++low) {
      const auto c = s[low];
      if (chars.find(c) != chars.end()) {
        if (indices[c]-- == 1) {
          indices.erase(c);
        }
      }
    }

    if (high - low + 1 < minSize) {
      minLow = low - 1;
      minSize = high - low + 1;
    }
  };

  return s.substr(minLow, minSize);
}

template <class Map, bool sort> void BM_string_set(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto [s, ss] = getTestCaseSS<sort>(state.range(0), state.range(1));
    state.ResumeTiming();
    benchmark::DoNotOptimize(
        find_smallest_substring_with<Map, StringSet>(s, ss));
  }
}

template <class Map, class Set> void BM_set(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto [s, ss] = getTestCaseS<Set>(state.range(0), state.range(1));
    state.ResumeTiming();
    benchmark::DoNotOptimize(find_smallest_substring_with<Map, Set>(s, ss));
  }
}

BENCHMARK_TEMPLATE(BM_string_set, std::unordered_map<char, size_t>, true)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_string_set, std::map<char, size_t>, true)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_string_set, std::unordered_map<char, size_t>, false)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_string_set, std::map<char, size_t>, false)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_set, std::unordered_map<char, size_t>, std::set<char>)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_set, std::map<char, size_t>, std::unordered_set<char>)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_set, std::unordered_map<char, size_t>,
                   std::unordered_set<char>)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});
BENCHMARK_TEMPLATE(BM_set, std::map<char, size_t>, std::set<char>)
    ->RangeMultiplier(2)
    ->Ranges({{kMinSize, kMaxSize}, {1, nChars}});

BENCHMARK_MAIN();
