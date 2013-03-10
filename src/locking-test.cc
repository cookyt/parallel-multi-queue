#include "queue.h"
#include "basic-test.h"

int main()
{
    cvl::ms::TwoLockQueue Q;
    BasicTest<cvl::ms::TwoLockQueue> test(Q, 100);
    test.run();
    return 0;
}
