#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"

#define MAX(a, b) a > b ? a : b;

extern template class cvl::MultiQueue<int>;
extern template class BasicTest<cvl::MultiQueue<int>, int>;

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

    int max_num_threads = MAX(num_producers, num_consumers)

    cvl::MultiQueue<int> Q(max_num_threads);
    BasicTest<cvl::MultiQueue<int>, int> test(Q, num_producers, num_consumers);
    test.run(10, 0);
    return 0;
}
