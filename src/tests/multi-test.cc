#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "queue.h"
#include "tests/basic-test.h"
#include "tests/parse-cmd-line.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// For large item tests
extern template class cvl::MultiQueue<std::vector<std::string> >;
extern template class BasicTest<cvl::MultiQueue<std::vector<std::string> >, std::vector<std::string> >;

// For small item tests
extern template class cvl::MultiQueue<int>;
extern template class BasicTest<cvl::MultiQueue<int>, int>;

int main(int argc, char **argv)
{
    using namespace std;
    using namespace cvl;
    using cvl::time::Time;

    CmdLineOpts opts;
    if (parseCmdLineOpts(argc, argv, opts) != 0)
        return 1;
    if (opts.verbose)
        opts.describe();

    int max_num_threads = MAX(opts.num_producers, opts.num_consumers);
    double secs, nsecs;
    int items;

    if (opts.use_large_test)
    {
        MultiQueue<vector<string> > Q(max_num_threads);
        BasicTest<MultiQueue<vector<string> >, vector<string> > test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);

        // Generate the "large" items. large vector of strings should do it.
        vector<string> product;
        for (int i=0; i<100; i++)
           product.push_back(string("Test String Contents"));

        pair<Time,int> throughput = test.run(product);
        secs = (double) throughput.first.secs;
        nsecs = (double) throughput.first.nsecs;
        items = throughput.second;
    }
    else
    {
        MultiQueue<int> Q(max_num_threads);
        BasicTest<MultiQueue<int>, int> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);

        pair<Time,int> throughput = test.run(0);
        secs = (double) throughput.first.secs;
        nsecs = (double) throughput.first.nsecs;
        items = throughput.second;
    }

    double time = secs + nsecs/1e9;
    if (opts.verbose)
        printf("throughput: %lf items/sec\n", items/time);
    else
        printf("%lf\n", items/time);

    return 0;
}
