#include <cstdio>
#include <cstdlib>
#include "queue.h"
#include "tests/basic-test.h"
#include "tests/parse-cmd-line.h"

extern template class cvl::ms::TwoLockQueue<int>;
extern template class BasicTest<cvl::ms::TwoLockQueue<int>, int>;

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
        fprintf(stderr, "Large items not implemented yet\n");
        return 1;
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
