#include <vector>
#include <string>
#include "tests/test-timed-throughput.h"
#include "queue/mq-counted.h"

extern template class mq::counted<std::vector<std::string> >;
extern template class mq::counted<int>;
template class test::timed_throughput<mq::counted<std::vector<std::string> >, std::vector<std::string> >;
template class test::timed_throughput<mq::counted<int>, int>;
