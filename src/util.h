#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

namespace cvl
{
    namespace atomic
    {
        uint32_t fetchAndAdd(uint32_t volatile *, uint32_t);
        uint32_t cas32(uint32_t *, uint32_t, uint32_t);
    }
}

#endif
