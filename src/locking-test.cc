#include "queue.h"
#include "basic-test.h"

int main()
{
    LockingQueue Q;
    BasicTest<LockingQueue> test(Q, 100);
    test.run();
    return 0;
}
