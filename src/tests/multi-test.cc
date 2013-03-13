#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"

extern template class cvl::MultiQueue<int>;
extern template class BasicTest<cvl::MultiQueue<int> >;

int main(int argc, char **argv)
{
    int num_threads;
    if (argc >= 2)
        num_threads = std::atoi(argv[1]);
    else
        return 1;

    cvl::MultiQueue<int> Q(num_threads);
    BasicTest<cvl::MultiQueue<int> > test(Q, num_threads);
    test.run();
    return 0;
}
