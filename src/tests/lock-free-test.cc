#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "queue.h"
#include "tests/basic-test.h"
#include "tests/parse-cmd-line.h"

// For small tests
extern template class cvl::ms::LockFreeQueue<int>;
extern template class BasicTest<cvl::ms::LockFreeQueue<int>, int>;

// For large tests
extern template class cvl::ms::LockFreeQueue<std::vector<std::string> >;
extern template class BasicTest<cvl::ms::LockFreeQueue<std::vector<std::string> >, std::vector<std::string> >;

int main(int argc, char **argv)
{
    using namespace std;
    using cvl::time::Time;
    using namespace cvl::ms;

    CmdLineOpts opts;
    if (parseCmdLineOpts(argc, argv, opts) != 0)
        return 1;
    if (opts.verbose)
        opts.describe();

    pair<Time,int> throughput;
    if (opts.use_large_test)
    {
        LockFreeQueue<vector<string> > Q;
        BasicTest<LockFreeQueue<vector<string> >, vector<string> > test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);

        // Generate the "large" items. large vector of strings should do it.
        vector<string> product;
        for (int i=0; i<100; i++)
           product.push_back(string("Test String Contents"));

        throughput = test.run(product);
    }
    else
    {
        LockFreeQueue<int> Q;
        BasicTest<LockFreeQueue<int>, int> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);
        throughput = test.run(0);
    }

    int items = throughput.second;
    double time = ((double) throughput.first.secs) + ((double) throughput.first.nsecs)/1e9;
    if (opts.verbose)
        printf("throughput: %lf items/sec\n", items/time);
    else
        printf("%lf\n", items/time);

    return 0;
}
