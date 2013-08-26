#include "util/util.h"

namespace util {

int nextPow2(int n) {
  int cur = 1;
  while (cur < n)
    cur = cur << 1;
  return cur;
}

void makeLargeItem(std::vector<std::string> *output) {
  for (int i=0; i<100; i++) {
    output->push_back(std::string("Test String Contents"));
  }
}

}  // namespace util
