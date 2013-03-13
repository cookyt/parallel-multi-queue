#include "tests/basic-test.h"
#include "queue.h"

template class BasicTest<cvl::ms::LockFreeQueue>;
template class BasicTest<cvl::ms::TwoLockQueue>;
template class BasicTest<cvl::MultiQueue>;
