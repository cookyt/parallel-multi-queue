#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

namespace cvl
{
    namespace atomic
    {
        uint32_t fetchAndAdd(uint32_t volatile *, uint32_t);
        bool cas64(uint64_t *, uint64_t, uint64_t);
    }
}

#endif
