/*
 * huge-alloc.cpp
 */

#include "huge-alloc.hpp"

#include <map>
#include <new>
#include <cstddef>
#include <cassert>
#include <cstring>

#include <sys/mman.h>
#include <linux/kernel-page-flags.h>

extern "C" {
#include "page-info.h"
}

using std::size_t;


struct alloc_info {
    /* the user-requested size */
    size_t user_size;
    /* the size we requested from mmap */
    size_t mmap_size;
    /* the pointer returned by mmap */
    void *mmap_p;
    explicit alloc_info(size_t user_size, size_t mmap_size, void *mmap_p) : user_size{user_size},
            mmap_size{mmap_size}, mmap_p{mmap_p} {}
};

std::map<void *, alloc_info> alloc_map;

class bad_huge_alloc : public std::bad_alloc {
    std::string msg;
public:
    bad_huge_alloc(std::string msg) : msg{std::move(msg)} {}
    const char *what() const throw() override {
        return msg.c_str();
    }
};

#define HUGE_PAGE_SIZE ((size_t)(2u * 1024u * 1024u))
#define HUGE_PAGE_MASK ((size_t)-HUGE_PAGE_SIZE)

/* allocate size bytes of storage in a hugepage */
void *huge_alloc(size_t user_size, bool print) {
    if (user_size > MAX_HUGE_ALLOC) {
        throw bad_huge_alloc("request exceeds MAX_HUGE_ALLOC, check your math");
    }

    // we request size + 2 * HUGE_PAGE_SIZE so we'll always have at least one huge page boundary in the allocation
    size_t mmap_size = user_size + 2 * HUGE_PAGE_SIZE;

    char *mmap_p = (char *)mmap(0, mmap_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (mmap_p == MAP_FAILED) {
        throw bad_huge_alloc("MAP_FAILED");
    }

    // align up to a hugepage boundary
    char *aligned_p = (char *)(((uintptr_t)mmap_p + HUGE_PAGE_SIZE) & HUGE_PAGE_MASK);

    madvise(aligned_p, user_size + HUGE_PAGE_SIZE, MADV_HUGEPAGE);

    // touch the memory so we can get stats on it
    std::memset(aligned_p, 0xCC, user_size);

    page_info_array  info   = get_info_for_range(aligned_p, aligned_p + user_size);
    flag_count fcount = get_flag_count(info, KPF_THP);
    if (print) fprintf(stderr, "hugepage ratio %4.3f (available %4.3f) for allocation of size %zu\n",
            (double)fcount.pages_set/fcount.pages_available,
            (double)fcount.pages_available/fcount.pages_total,
            user_size);

    assert(alloc_map.find(aligned_p) == alloc_map.end());
    // fprintf(stderr, "adding aligned_p=%p to the map\n", aligned_p);
    alloc_map.emplace(aligned_p, alloc_info{user_size, mmap_size, mmap_p});
    return aligned_p;
}

/* free the pointer pointed to by p */
void huge_free(void *p) {
    // fprintf(stderr, "huge_free for  p=%p\n", p);
    auto found = alloc_map.find(p);
    if (found == alloc_map.end()) {
        throw bad_huge_alloc("attempt to free not-allocated pointer");
    }
    alloc_info info = found->second;
    munmap(info.mmap_p, info.mmap_size);
    alloc_map.erase(found);
}
