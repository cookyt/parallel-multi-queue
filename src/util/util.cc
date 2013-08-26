#include "util/util.h"

namespace util {

int nextPow2(int n) {
  int cur = 1;
  while (cur < n)
    cur = cur << 1;
  return cur;
}

}  // namespace util
