#include "common.hpp"

#include <algorithm>
#include <stdlib.h>
#include <inttypes.h>

/**
 * The sorts offered by the C and C++ standard library.
 */

void qsort_copy(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

// int compare_uint64_t(const void *l_, const void *r_) { 
//     uint64_t l = *(const uint64_t *)l_;                                
//     uint64_t r = *(const uint64_t *)r_;                                
//     return (l > r) - (l < r);                            
// }                                    
int compare_uint64_t(const void *l_, const void *r_) { 
    uint64_t l = *(const uint64_t *)l_;
    uint64_t r = *(const uint64_t *)r_;
    if (l < r) return -1;
    if (l > r) return  1;
    return 0;
}

void qsort_sort(uint64_t* input, size_t size) {
    qsort(input, size, sizeof(uint64_t), compare_uint64_t);
}

/**
 * The source for qsort, but recompiled locally with the current compiler flags, etc.
 */
void qsort_recompiled(uint64_t* input, size_t size) {
    qsort_copy(input, size, sizeof(uint64_t), compare_uint64_t);
}

void std_sort(uint64_t* input, size_t size) {
    return std::sort(input, input + size);
}

void std_stable_sort(uint64_t* input, size_t size) {
    return std::stable_sort(input, input + size);
}

void std_partial_sort(uint64_t* input, size_t size) {
    return std::partial_sort(input, input + size, input + size);
}
