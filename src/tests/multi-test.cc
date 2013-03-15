#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "queue.h"
#include "tests/basic-test.h"

#define MAX(a, b) a > b ? a : b;

extern template class cvl::MultiQueue<std::vector<std::string> >;
extern template class BasicTest<cvl::MultiQueue<std::vector<std::string> >, std::vector<std::string> >;

int main(int argc, char **argv)
{
    using namespace std;
    int num_producers, num_consumers, time_to_run;
    if (argc >= 3)
    {
        num_producers = std::atoi(argv[1]);
        num_consumers = std::atoi(argv[2]);
        time_to_run   = std::atoi(argv[3]);
    }
    else
    {
        std::fprintf(stderr, "format: %s <number of producers> "
            "<number of consumers> <time to run the simulation>\n",
            argv[0]);
        return 1;
    }

    int max_num_threads = MAX(num_producers, num_consumers)

    cvl::MultiQueue<vector<string> > Q(max_num_threads);
    BasicTest<cvl::MultiQueue<vector<string> >, vector<string> > test(Q, num_producers, num_consumers, time_to_run);

    vector<string> product;
    for (int i=0; i<100; i++)
        product.push_back(string("Test String Contents"));
    std::pair<cvl::time::Time, int> throughput = test.run(product);

    double time = (double) throughput.first.secs + (double)throughput.first.nsecs/(double)1e9;
    std::printf("throughput: %lf items/sec\n", throughput.second/time);
    return 0;
}
