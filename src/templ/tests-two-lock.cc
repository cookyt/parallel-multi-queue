#include "tests/basic-test.h"
#include "queue/ms-two-lock-queue.h"

extern template class cvl::ms::TwoLockQueue<int>;
template class BasicTest<cvl::ms::TwoLockQueue<int>, int>;
