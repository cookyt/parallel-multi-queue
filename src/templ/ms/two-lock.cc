#include <vector>
#include <string>

#include "queue/ms/two-lock.h"

template class ms::two_lock<std::vector<std::string>>;
template class ms::two_lock<int>;
