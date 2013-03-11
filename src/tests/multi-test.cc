#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"

int main(int argc, char **argv)
{
    int num_threads;
    if (argc >= 2)
        num_threads = std::atoi(argv[1]);
    else
        return 1;

    cvl::MultiQueue Q(num_threads);
    BasicTest<cvl::MultiQueue> test(Q, num_threads);
    test.run();
    return 0;
}
