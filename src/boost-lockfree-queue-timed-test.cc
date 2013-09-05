#include <iostream>
#include <string>
#include <vector>

#include <boost/lockfree/queue.hpp>

#include "test/timed-throughput-fixture.h"
#include "test/timed-throughput.h"
#include "util/parse-cmd-line.h"
#include "util/util.h"

using boost::lockfree::queue;
using std::string;
using std::vector;
using test::timed_throughput;
using test::timed_throughput_fixture;
using util::CmdLineOpts;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef timed_throughput_fixture<queue<int>, int,
                                 queue<int>, int> Fixture;

int main(int argc, char **argv) {
  CmdLineOpts opts;
  if (parseCmdLineOpts(argc, argv, opts) != 0)
    return 1;

  if (opts.use_large_test) {
    std::cerr << "Large tests not supported" << std::endl;
    return 1;
  }

  int max_num_threads = MAX(opts.num_producers, opts.num_consumers);
  boost::lockfree::queue<int> small_queue(max_num_threads);
  int small_item = 0;

  Fixture fixture(&small_queue, &small_item, &small_queue, &small_item);
  if (fixture.run(argc, argv))
    return 0;
  else
    return 1;
}
