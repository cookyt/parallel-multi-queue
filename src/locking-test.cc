#include "queue.h"
#include "basic-test.h"

int main()
{
    ms::TwoLockQueue Q;
    BasicTest<ms::TwoLockQueue> test(Q, 100);
    test.run();
    return 0;
}
