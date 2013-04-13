#include <vector>
#include <string>
#include "tests/basic-test.h"
#include "queue/multi-queue.h"

extern template class cvl::MultiQueue<std::vector<std::string> >;
extern template class cvl::MultiQueue<int>;
template class BasicTest<cvl::MultiQueue<std::vector<std::string> >, std::vector<std::string> >;
template class BasicTest<cvl::MultiQueue<int>, int>;
