#ifndef UTIL_TIME_H
#define UTIL_TIME_H

namespace util {
namespace time {

// Represents a single point in time using the best resolution available for the
// current archetecture.
struct Time {
  // Initializes to 0 time passed since Unix epoch
  Time();

  Time(uint64_t secs_, uint64_t nsecs_);
  Time(const Time &);

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

#endif  // UTIL_TIME_H
