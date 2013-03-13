#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"

extern template class cvl::ms::LockFreeQueue<int>;
extern template class BasicTest<cvl::ms::LockFreeQueue<int> >;

int main(int argc, char **argv)
{
    int num_threads;
    if (argc >= 2)
        num_threads = std::atoi(argv[1]);
    else
        return 1;

    cvl::ms::LockFreeQueue<int> Q;
    BasicTest<cvl::ms::LockFreeQueue<int> > test(Q, num_threads);
    test.run();
    return 0;
}
