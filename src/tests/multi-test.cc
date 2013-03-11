#include "queue.h"
#include "tests/basic-test.h"

int main()
{
    cvl::MultiQueue Q(1000);
    BasicTest<cvl::MultiQueue> test(Q, 1000);
    test.run();
    return 0;
}
