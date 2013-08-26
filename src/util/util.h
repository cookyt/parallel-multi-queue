#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

namespace util {

// In bytes. Usually 64. Found in file:
// /sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size
const int kCacheLineSize = 64;

/**
 * Calculates the next power of two larger than the input.
 */
int nextPow2(int n);

}  // namespace util

#endif // UTIL_H
