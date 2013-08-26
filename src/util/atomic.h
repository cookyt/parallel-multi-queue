#ifndef UTIL_ATOMIC_H
#define UTIL_ATOMIC_H

#include <stdint.h>

/**
 * Defines some simple atomic primatives. Hold-over from the C++03
 * implementation.
 *
 * TODO(cookyt): All uses of this header should be replaced with C++11 atomic
 * primatives.
 */

namespace util {
namespace atomic {

/**
 * This adds the given value to the word at the specified location and returns
 * the value prior to the addition.
 */
inline uint32_t fetchAndAdd(uint32_t volatile *addr, uint32_t val) {
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
template<typename T>
bool cas(T *addr, T oldval, T newval) {
  return __sync_bool_compare_and_swap(addr, oldval, newval);
}

}  // namespace atomic
}  // namespace util

#endif  // UTIL_ATOMIC_H
