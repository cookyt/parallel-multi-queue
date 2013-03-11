#include "queue.h"
#include "tests/basic-test.h"

int main()
{
    cvl::ms::TwoLockQueue Q;
    BasicTest<cvl::ms::TwoLockQueue> test(Q, 1000);
    test.run();
    return 0;
}
