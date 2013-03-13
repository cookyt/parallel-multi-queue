#include "util.h"

/**
 * This adds the given value to the word at the specified location and returns
 * the value prior to the addition.
 *
 * Uses a GCC-specific bultin. Should be implemented in a more portable
 * fashion.
 */
uint32_t cvl::atomic::fetchAndAdd(uint32_t volatile *addr, uint32_t val)
{
    return __sync_fetch_and_add(addr, val);
}

/**
 * Atomically compares the value at the given memory location to the given
 * oldval and replaces it with newval if they are the same. Returns the value
 * that was found in the memory location.
 *
 * Uses a GCC-specific bultin. Should be implemented in a more portable
 * fashion.
 */
bool cvl::atomic::cas64(uint64_t *addr, uint64_t oldval, uint64_t newval)
{
    return __sync_bool_compare_and_swap(addr, oldval, newval);
}

int cvl::nextPow2(int n)
{
    int cur = 1;
    while (cur < n)
        cur = cur << 1;
    return cur;
}

