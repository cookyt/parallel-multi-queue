#include <stdint.h>
#include <time.h>

#include "util/time.h"

namespace util {
namespace time {

Time::Time(uint64_t secs_, uint64_t nsecs_)
    : secs(secs_), nsecs(nsecs_) {}

Time::Time(const Time &time)
    : secs(time.secs), nsecs(time.nsecs) {}

Time::Time()
    : secs(0), nsecs(0) {}

Time &Time::operator-=(const Time &time) {
  secs -= time.secs;
  nsecs -= time.nsecs;
  if (nsecs < 0) {
    secs -= 1;
    nsecs += 1000000000;
  }
  return *this;
}

Time &Time::operator+=(const Time &time) {
  secs += time.secs;
  nsecs += time.nsecs;
  if (nsecs >= 1000000000) {
    secs += 1;
    nsecs -= 1000000000;
  }
  return *this;
}

Time &Time::operator=(const Time &time) {
  secs = time.secs;
  nsecs = time.nsecs;
  return *this;
}

const Time &Time::operator-(const Time &time) {
  return Time(*this) -= time;
}

const Time &Time::operator+(const Time &time) {
  return Time(*this) += time;
}

Time now_cpu() {
  struct timespec time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
  return Time(time.tv_sec, time.tv_nsec);
}

Time now_real() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return Time(time.tv_sec, time.tv_nsec);
}

}  // namespace time
}  // namespace util
