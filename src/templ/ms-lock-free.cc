#include <vector>
#include <string>
#include "queue/ms-lock-free.h"

template class ms::lock_free<std::vector<std::string> >;
template class ms::lock_free<int>;
