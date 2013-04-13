#include <cstdlib>
#include <cstdio>
#include "queue.h"
#include "tests/basic-test.h"

extern template class cvl::ms::LockFreeQueue<int>;
extern template class BasicTest<cvl::ms::LockFreeQueue<int>, int>;

int main(int argc, char **argv)
{
    int num_producers, num_consumers, time_to_run;
    if (argc >= 4)
    {
        num_producers = std::atoi(argv[1]);
        num_consumers = std::atoi(argv[2]);
        time_to_run   = std::atoi(argv[3]);
    }
    else
    {
        return 1;
    }

    cvl::ms::LockFreeQueue<int> Q;
    BasicTest<cvl::ms::LockFreeQueue<int>, int> test(Q, num_producers, num_consumers, time_to_run);
    std::pair<cvl::time::Time, int> throughput = test.run(0);

    double time = (double) throughput.first.secs + (double)throughput.first.nsecs/(double)1e9;
    std::printf("throughput: %lf items/sec\n", throughput.second/time);

    return 0;
}
