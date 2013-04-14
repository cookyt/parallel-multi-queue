#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

namespace cvl
{
    namespace atomic
    {
        uint32_t fetchAndAdd(uint32_t volatile *, uint32_t);
        bool cas64(uint64_t *, uint64_t, uint64_t);
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

    int nextPow2(int n);
}

#endif
