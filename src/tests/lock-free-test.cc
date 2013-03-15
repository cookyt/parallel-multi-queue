#include <cstdlib>
#include <cstdio>
#include "queue.h"
#include "tests/basic-test.h"

extern template class cvl::ms::LockFreeQueue<int>;
extern template class BasicTest<cvl::ms::LockFreeQueue<int>, int>;

int main(int argc, char **argv)
{
    // int num_threads;
    // if (argc >= 2)
    //     num_threads = std::atoi(argv[1]);
    // else
    //     return 1;

    // cvl::ms::LockFreeQueue<int> Q;
    // BasicTest<cvl::ms::LockFreeQueue<int> > test(Q, num_threads);
    // test.run();
    fprintf(stderr, "Function currently not working.");
    return 0;
}
