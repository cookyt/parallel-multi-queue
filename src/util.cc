#include <time.h>
#include "util.h"

/**
 * Most of the things in this CU are wrappers arounf lower-level functions. The
 * main use of this library is to make porting easier.
 */

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

cvl::time::Time::Time(uint64_t secs_, uint64_t nsecs_) : 
    secs(secs_), nsecs(nsecs_)
{}

cvl::time::Time::Time(const Time &time) :
    secs(time.secs), nsecs(time.nsecs)
{}

cvl::time::Time::Time() :
    secs(0), nsecs(0)
{}

cvl::time::Time &cvl::time::Time::operator-=(const Time &time)
{
    secs -= time.secs;
    nsecs -= time.nsecs;
    if (nsecs < 0)
    {
        secs -= 1;
        nsecs += 1000000000;
    }
    return *this;
}

cvl::time::Time &cvl::time::Time::operator+=(const Time &time)
{
    secs += time.secs;
    nsecs += time.nsecs;
    if (nsecs >= 1000000000)
    {
        secs += 1;
        nsecs -= 1000000000;
    }
    return *this;
}

cvl::time::Time &cvl::time::Time::operator=(const Time &time)
{
    secs = time.secs;
    nsecs = time.nsecs;
    return *this;
}

const cvl::time::Time &cvl::time::Time::operator-(const Time &time)
{
    return Time(*this) -= time;
}

const cvl::time::Time &cvl::time::Time::operator+(const Time &time)
{
    return Time(*this) += time;
}

/**
 * Gets the current time starting from the beginning of the process. Note, this
 * is user-space time, so concurrent threads will make it longer than the
 * wall-clock time
 */
cvl::time::Time cvl::time::now_cpu()
{
    struct timespec time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
    return Time(time.tv_sec, time.tv_nsec);
}

/**
 * Gets the real (wall-clock) time. Time zero is some arbitrary start point in
 * the past. The actual value returned from this isn't too useful, but the
 * difference between times from this function can measure the time elapsed in
 * a section of code.
 */
cvl::time::Time cvl::time::now_real()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return Time(time.tv_sec, time.tv_nsec);
}

/**
 * Calculates the next power of two larger than the input.
 */
int cvl::nextPow2(int n)
{
    int cur = 1;
    while (cur < n)
        cur = cur << 1;
    return cur;
}
