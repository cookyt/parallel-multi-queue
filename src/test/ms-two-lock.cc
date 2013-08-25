#include <cstdio>
#include <cstdlib>
#include "queue.h"

#include "parse-cmd-line.h"
#include "test/timed-throughput.h"

// For small tests
extern template class ms::two_lock<int>;
extern template class test::timed_throughput<ms::two_lock<int>, int>;

// For large tests
extern template class ms::two_lock<std::vector<std::string>>;
extern template class test::timed_throughput<ms::two_lock<std::vector<std::string>>, std::vector<std::string>>;

int main(int argc, char **argv) {
  using namespace std;
  using util::time::Time;

  CmdLineOpts opts;
  if (parseCmdLineOpts(argc, argv, opts) != 0)
    return 1;
  if (opts.verbose)
    opts.describe();

  pair<Time,int> throughput;
  if (opts.use_large_test) {
    ms::two_lock<vector<string>> Q;
    test::timed_throughput<ms::two_lock<vector<string>>, vector<string>> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);

    // Generate the "large" items. large vector of strings should do it.
    vector<string> product;
    for (int i=0; i<100; i++)
       product.push_back(string("Test String Contents"));

    throughput = test.run(product);
  } else {
    ms::two_lock<int> Q;
    test::timed_throughput<ms::two_lock<int>, int> test(Q, opts.num_producers, opts.num_consumers, opts.time_to_run);
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
