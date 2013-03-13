#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"

extern template class cvl::ms::TwoLockQueue<int>;
extern template class BasicTest<cvl::ms::TwoLockQueue<int>, int>;

int main(int argc, char **argv)
{
    int num_producers, num_consumers;
    if (argc >= 3)
    {
        num_producers = std::atoi(argv[1]);
        num_consumers = std::atoi(argv[2]);
    }
    else
    {
        return 1;
    }

    cvl::ms::TwoLockQueue<int> Q;
    BasicTest<cvl::ms::TwoLockQueue<int>, int> test(Q, num_producers, num_consumers);
    test.run(10, 0);
    return 0;
}
