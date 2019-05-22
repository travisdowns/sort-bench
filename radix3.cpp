#include "common.hpp"

#include <algorithm>
#include <array>
#include <vector>
#include <assert.h>
#include <string.h>

const size_t    RADIX_BITS   = 8;
const size_t    RADIX_SIZE   = (size_t)1 << RADIX_BITS;
const size_t    RADIX_LEVELS = (63 / RADIX_BITS) + 1;
const uint64_t  RADIX_MASK   = RADIX_SIZE - 1;

void radix_sort3(uint64_t *a, size_t count)
{
    using queuetype = std::vector<uint64_t>;

    std::array<queuetype, RADIX_SIZE> queues;

    for (auto& queue : queues) {
        queue.reserve(count / RADIX_SIZE * 1.2);
    }
    
    for (size_t pass = 0; pass < RADIX_LEVELS; pass++) {
        uint64_t shift = pass * RADIX_BITS;

        // copy each element into the appropriate queue based on the current RADIX_BITS sized
        // "digit" within it
        for (size_t i = 0; i < count; i++) {
            size_t value = a[i];
            size_t index = (value >> shift) & RADIX_MASK;
            queues[index].push_back(value);
        }

        // copy all the queues back over top of the original array in order
        uint64_t* aptr = a;
        for (auto& queue : queues) {
            aptr = std::copy(queue.begin(), queue.end(), aptr);
            queue.clear();
        }
    }
}
