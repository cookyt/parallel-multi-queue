#include "tests/basic-test.h"
#include "queue/multi-queue.h"

extern template class cvl::MultiQueue<int>;
template class BasicTest<cvl::MultiQueue<int> >;
