#include "queue.h"
#include "basic-test.h"

int main()
{
    cvl::MultiQueue Q(100);
    BasicTest<cvl::MultiQueue> test(Q, 100);
    test.run();
    return 0;
}
