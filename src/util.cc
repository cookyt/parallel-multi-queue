#include <time.h>
#include "util.h"

/**
 * Uses a GCC-specific bultin. Should be implemented in a more portable
 * fashion.
 */
uint32_t util::atomic::fetchAndAdd(uint32_t volatile *addr, uint32_t val) {
  return __sync_fetch_and_add(addr, val);
}

util::time::Time::Time(uint64_t secs_, uint64_t nsecs_)
    : secs(secs_), nsecs(nsecs_) {}

util::time::Time::Time(const Time &time)
    : secs(time.secs), nsecs(time.nsecs) {}

util::time::Time::Time()
    : secs(0), nsecs(0) {}

util::time::Time &util::time::Time::operator-=(const Time &time) {
  secs -= time.secs;
  nsecs -= time.nsecs;
  if (nsecs < 0) {
    secs -= 1;
    nsecs += 1000000000;
  }
  return *this;
}

util::time::Time &util::time::Time::operator+=(const Time &time) {
  secs += time.secs;
  nsecs += time.nsecs;
  if (nsecs >= 1000000000) {
    secs += 1;
    nsecs -= 1000000000;
  }
  return *this;
}

util::time::Time &util::time::Time::operator=(const Time &time) {
  secs = time.secs;
  nsecs = time.nsecs;
  return *this;
}

const util::time::Time &util::time::Time::operator-(const Time &time) {
  return Time(*this) -= time;
}

const util::time::Time &util::time::Time::operator+(const Time &time) {
  return Time(*this) += time;
}

util::time::Time util::time::now_cpu() {
  struct timespec time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
  return Time(time.tv_sec, time.tv_nsec);
}

util::time::Time util::time::now_real() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return Time(time.tv_sec, time.tv_nsec);
}

int util::nextPow2(int n) {
  int cur = 1;
  while (cur < n)
    cur = cur << 1;
  return cur;
}
