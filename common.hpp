#ifndef COMMON_H_
#define COMMON_H_

#include <inttypes.h>
#include <stddef.h>

/** something that can sort in-place input with the given size */
using sort_fn = void (uint64_t* input, size_t size);

/**
 * A sort algorithm with a name.
 */
struct sort_algo {
    const char* name;
    sort_fn* fn;
};

// all the sorts
sort_fn qsort_sort;
sort_fn qsort_recompiled;
sort_fn qsort_inlined;
sort_fn std_sort;
sort_fn std_stable_sort;
sort_fn std_partial_sort;
sort_fn radix_sort1;
sort_fn radix_sort2;
sort_fn radix_sort3;
sort_fn radix_sort4;
sort_fn radix_sort5;
sort_fn radix_sort6;
sort_fn radix_sort7;
sort_fn radix_sort8;
sort_fn radix_sort9;
sort_fn radix_sort10;

sort_fn radix_sortX;


#endif // #ifndef COMMON_H_
