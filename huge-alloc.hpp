/*
 * huge-alloc.hpp
 *
 * Inefficient allocator that allows allocating memory regions backed by THP pages.
 */

#ifndef HUGE_ALLOC_HPP_
#define HUGE_ALLOC_HPP_

#include <cstddef>
#include <memory>

// 1152921504606846976 bytes should be enough for everyone
#define MAX_HUGE_ALLOC (1ULL << 60)

/* allocate size bytes of storage in a hugepage */
void *huge_alloc(std::size_t size, bool print);

/* free the pointer pointed to by p */
void huge_free(void *p);

struct huge_deleter {
    void operator()(void *p) {
        huge_free(p);
    }
};

template <typename T>
using huge_unique_ptr = std::unique_ptr<T, huge_deleter>;

template <typename T>
huge_unique_ptr<T[]> make_huge_array(size_t count, bool print) {
    return huge_unique_ptr<T[]>((T*)huge_alloc(count * sizeof(T), print));
}

#endif /* HUGE_ALLOC_HPP_ */
