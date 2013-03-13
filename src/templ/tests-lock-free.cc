#include "tests/basic-test.h"
#include "queue/ms-lock-free-queue.h"

extern template class cvl::ms::LockFreeQueue<int>;
template class BasicTest<cvl::ms::LockFreeQueue<int>, int>;
