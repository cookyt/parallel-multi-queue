#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "parse-cmd-line.h"
#include "queue.h"
#include "test/timed-throughput.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// For large item tests
extern template class mq::counted<std::vector<std::string>>;
extern template class test::timed_throughput<mq::counted<std::vector<std::string>>, std::vector<std::string>>;

// For small item tests
extern template class mq::counted<int>;
extern template class test::timed_throughput<mq::counted<int>, int>;

int main(int argc, char **argv) {
  using namespace std;
  using util::time::Time;

  CmdLineOpts opts;
  if (parseCmdLineOpts(argc, argv, opts) != 0)
    return 1;
  if (opts.verbose)
    opts.describe();

  int max_num_threads = MAX(opts.num_producers, opts.num_consumers);

  pair<Time,int> throughput;
  if (opts.use_large_test) {
    mq::counted<vector<string>> Q(max_num_threads);
    test::timed_throughput<mq::counted<vector<string>>, vector<string>> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);

    // Generate the "large" items. large vector of strings should do it.
    vector<string> product;
    for (int i=0; i<100; i++)
       product.push_back(string("Test String Contents"));

    throughput = test.run(product);
  } else {
    mq::counted<int> Q(max_num_threads);
    test::timed_throughput<mq::counted<int>, int> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);
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
