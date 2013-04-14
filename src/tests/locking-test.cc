#include <cstdio>
#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"
#include "tests/parse-cmd-line.h"

// For small tests
extern template class cvl::ms::TwoLockQueue<int>;
extern template class BasicTest<cvl::ms::TwoLockQueue<int>, int>;

// For large tests
extern template class cvl::ms::TwoLockQueue<std::vector<std::string> >;
extern template class BasicTest<cvl::ms::TwoLockQueue<std::vector<std::string> >, std::vector<std::string> >;

int main(int argc, char **argv)
{
    using namespace std;
    using namespace cvl::ms;
    using cvl::time::Time;

    CmdLineOpts opts;
    if (parseCmdLineOpts(argc, argv, opts) != 0)
        return 1;
    if (opts.verbose)
        opts.describe();

    pair<Time,int> throughput;
    if (opts.use_large_test)
    {
        TwoLockQueue<vector<string> > Q;
        BasicTest<TwoLockQueue<vector<string> >, vector<string> > test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);

        // Generate the "large" items. large vector of strings should do it.
        vector<string> product;
        for (int i=0; i<100; i++)
           product.push_back(string("Test String Contents"));

        throughput = test.run(product);
    }
    else
    {
        TwoLockQueue<int> Q;
        BasicTest<TwoLockQueue<int>, int> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);
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
