#include "multi-queue.h"
#include "basic-test.h"

int main()
{
    MultiQueue Q(100);
    BasicTest<MultiQueue> test(Q, 100);
    test.run();
    return 0;
}
