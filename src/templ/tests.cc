#include "tests/basic-test.h"
#include "queue.h"

extern template class cvl::ms::LockFreeQueue<int>;
extern template class cvl::ms::TwoLockQueue<int>;
extern template class cvl::MultiQueue<int>;

template class BasicTest<cvl::ms::LockFreeQueue<int> >;
template class BasicTest<cvl::ms::TwoLockQueue<int> >;
template class BasicTest<cvl::MultiQueue<int> >;
