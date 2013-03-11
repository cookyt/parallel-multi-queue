#include <cstdio>
#include <boost/thread.hpp>
#include "queue.h"
#include "tests/basic-test.h"

int main()
{
    cvl::ms::LockFreeQueue Q;
    BasicTest<cvl::ms::LockFreeQueue> test(Q, 1000);
    test.run();

    return 0;
}
