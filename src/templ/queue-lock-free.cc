#include <vector>
#include <string>
#include "queue/ms-lock-free-queue.h"

template class cvl::ms::LockFreeQueue<std::vector<std::string> >;
template class cvl::ms::LockFreeQueue<int>;
