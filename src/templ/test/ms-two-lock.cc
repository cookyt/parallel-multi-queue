#include <vector>
#include <string>
#include "test/timed-throughput.h"
#include "queue/ms/two-lock.h"

extern template class ms::two_lock<int>;
template class test::timed_throughput<ms::two_lock<int>, int>;

extern template class ms::two_lock<std::vector<std::string>>;
template class test::timed_throughput<ms::two_lock<std::vector<std::string>>, std::vector<std::string>>;
