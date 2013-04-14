#include <vector>
#include <string>
#include "tests/basic-test.h"
#include "queue/ms-lock-free-queue.h"

extern template class cvl::ms::LockFreeQueue<int>;
template class BasicTest<cvl::ms::LockFreeQueue<int>, int>;

extern template class cvl::ms::LockFreeQueue<std::vector<std::string> >;
template class BasicTest<cvl::ms::LockFreeQueue<std::vector<std::string> >, std::vector<std::string> >;
