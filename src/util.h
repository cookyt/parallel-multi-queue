#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

/**
 * File utilities and contains wrappers to lower-level functions. Putting these
 * things here makes it easier to port the code later.
 */

namespace util {

// In bytes. Usually 64. Found in file:
// /sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size
const int kCacheLineSize = 64;

/**
 * Calculates the next power of two larger than the input.
 */
int nextPow2(int n);

namespace atomic {

/**
 * This adds the given value to the word at the specified location and returns
 * the value prior to the addition.
 */
uint32_t fetchAndAdd(uint32_t volatile *addr, uint32_t val);

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

namespace time {

// Represents a single point in time using the best resolution available.
struct Time {
  Time(uint64_t secs_, uint64_t nsecs_);
  Time(const Time &);
  Time();

  Time &operator-=(const Time &);
  Time &operator+=(const Time &);
  Time &operator=(const Time &);
  const Time &operator-(const Time &);
  const Time &operator+(const Time &);

  uint64_t secs;
  uint64_t nsecs;
};

/**
 * Gets the current time starting from the beginning of the process. Note, this
 * is user-space time, so concurrent threads will make it longer than the
 * wall-clock time
 */
Time now_cpu();


/**
 * Gets the real (wall-clock) time. Time zero is some arbitrary start point in
 * the past. The actual value returned from this isn't too useful, but the
 * difference between times from this function can measure the time elapsed in
 * a section of code.
 */
Time now_real();

}  // namespace time
}  // namespace util

#endif // UTIL_H
