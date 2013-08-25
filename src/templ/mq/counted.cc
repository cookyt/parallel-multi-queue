#include <vector>
#include <string>
#include "queue/mq/counted.h"

template class mq::counted<std::vector<std::string>>;
template class mq::counted<int>;
