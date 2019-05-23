#include "common.hpp"
#include "wyhash.h"
#include "simple-timer.hpp"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <algorithm>
#include <vector>
#include <memory>

inline uint64_t random_range(uint64_t range) {
    // Fast Random Integer Generation in an Interval,  
    // ACM Transactions on Modeling and Computer Simulation 29 (1), 2019
    return (uint64_t) (((__uint128_t)wyhash64() * range) >> 64);
}

bool is_sorted(const uint64_t* p, size_t size) {
  for (size_t i = 0; i + 1 < size; i++) {
    if (p[i] > p[i + 1]) {
      return false;
    }
  }
  return true;
}

void fill_N(uint64_t range_min, uint64_t range_max, size_t N, uint64_t *out) {
  uint64_t range_size = range_max - range_min;
  for(size_t i = 0; i < N; i++) {
    out[i] = random_range(range_size) + range_min;
  }
}

/** fill array with repeating sequence of 0..stop inclusive */
void fill_linear(uint64_t stop, size_t N, uint64_t *out) {
  for (size_t i = 0; i < N;) {
    for (uint64_t c = 0, v = 0; c <= stop && i < N; v = (v + 17) & 0xFF, i++) {
      out[i] = v * 0x0101010101010101;
    }
  }
}

#ifndef SIZE
const int repeats = 2;
const size_t min_size  = 1000ull;
const size_t max_size  = 10000000ull;
#else 
const int repeats_ = 1000000 / SIZE;  // normally just 2, one for warmup, one measured
const int repeats = repeats_ > 0 ? repeats_ : 1;
const size_t min_size  = SIZE;
const size_t max_size  = min_size;
#endif

const size_t range_max = 40000000000ull;
// const size_t range_max = 0xFFFFFFFFFFFFFFFFull;

void bench_one(const sort_algo& algo, const uint64_t* input, uint64_t* buffer, size_t size) {
    // assert(!is_sorted(input, size));

    // clock_t before, after;
    int64_t elapsed;
    for (int i = 0; i < repeats; i++) {
      std::copy(input, input + size, buffer);
      
      // before = clock();
      SimpleTimer timer;
      algo.fn(buffer, size);
      elapsed = timer.elapsedNanos();
      // after  = clock();
    }

    assert(is_sorted(buffer, size));

    // printf(",%*.1f", (int)strlen(algo.name), (after - before) * 1000000000. / size / CLOCKS_PER_SEC);
    printf(",%*.1f", (int)strlen(algo.name), (double)elapsed / size);

}

int main(int argc, char** argv) {

  std::vector<sort_algo> all_algos = {
    {"qsort", qsort_sort},
    {"qsort-inlined", qsort_inlined},
    {"std::sort", std_sort},
    {"std::stable_sort", std_stable_sort},
    {"std::partial_sort", std_partial_sort},
    {"radix1", radix_sort1},
    {"radix2", radix_sort2},
    {"radix3", radix_sort3},
    {"radix4", radix_sort4},
    {"radix5", radix_sort5},
    {"radix6", radix_sort6},
    {"radix7", radix_sort7},
    {"qsort-recompiled", qsort_recompiled},
  }, selected_algos;

  if (argc > 1) {
    // test name provided, look for that test
    const std::string test_name = argv[1];
    for (auto& algo : all_algos) {
      if (test_name == algo.name) {
        selected_algos.push_back(algo);
      }
    }
    if (selected_algos.empty()) {
      fprintf(stderr, "No test named %s\n", test_name.c_str());
      exit(1);
    }
  } else {
    selected_algos = all_algos;
  }

  wyhash64_seed(0x1234567890ABCDEFull);

  uint64_t* temp = new uint64_t[max_size];
  fill_N(0, range_max, max_size, temp);
  const uint64_t* input = temp;

  // allocate a temporary buffer of the same size as the input array (max_size),
  // which the benchmark function uses as the input to each algorithm (since we)
  // won't want to destroy the original
  auto temp_buffer = std::unique_ptr<uint64_t[]>(new uint64_t[max_size]);
  // auto temp_buffer = make_huge_array<uint64_t>(max_size, true);

  int size_width = std::to_string(max_size).size();
  printf("%*s", size_width, "size");
  for (auto& algo : selected_algos) {
    printf(",%s", algo.name);
  }

  for (size_t size = min_size; size <= max_size; size *= 10) {
    printf("\n%*zu", size_width, size);
    for (auto& algo : selected_algos) {
      bench_one(algo, input, temp_buffer.get(), size);
    }
  }
  printf("\n");
}
