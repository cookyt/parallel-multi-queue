#include <vector>
#include <string>
#include "test/timed-throughput.h"
#include "queue/ms/lock-free.h"

extern template class ms::lock_free<int>;
template class test::timed_throughput<ms::lock_free<int>, int>;

extern template class ms::lock_free<std::vector<std::string>>;
template class test::timed_throughput<ms::lock_free<std::vector<std::string>>, std::vector<std::string>>;
