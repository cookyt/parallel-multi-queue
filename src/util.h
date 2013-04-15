#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

namespace cvl
{
    namespace atomic
    {
        uint32_t fetchAndAdd(uint32_t volatile *, uint32_t);

        /**
         * Atomically compares the value at the given memory location to the given
         * oldval and replaces it with newval if they are the same. Returns the value
         * that was found in the memory location.
         *
         * Uses a GCC-specific bultin. Should be implemented in a more portable
         * fashion.
         */
        template<typename T>
        bool cas(T *addr, T oldval, T newval)
        {
            return __sync_bool_compare_and_swap(addr, oldval, newval);
        }
    }

    namespace time
    {
        struct Time
        {
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

        Time now_cpu();
        Time now_real();
    }

    // In bytes. Found in file:
    // /sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size
    const int kCacheLineSize = 64;
    int nextPow2(int n);
}

#endif
