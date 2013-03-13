#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"

extern template class BasicTest<cvl::ms::LockFreeQueue>;

int main(int argc, char **argv)
{
    int num_threads;
    if (argc >= 2)
        num_threads = std::atoi(argv[1]);
    else
        return 1;

    cvl::ms::LockFreeQueue Q;
    BasicTest<cvl::ms::LockFreeQueue> test(Q, num_threads);
    test.run();
    return 0;
}
