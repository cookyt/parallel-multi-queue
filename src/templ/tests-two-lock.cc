#include <vector>
#include <string>
#include "tests/basic-test.h"
#include "queue/ms-two-lock-queue.h"

extern template class cvl::ms::TwoLockQueue<int>;
template class BasicTest<cvl::ms::TwoLockQueue<int>, int>;

extern template class cvl::ms::TwoLockQueue<std::vector<std::string> >;
template class BasicTest<cvl::ms::TwoLockQueue<std::vector<std::string> >, std::vector<std::string> >;
