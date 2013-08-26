#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

#include "queue.h"
#include "test/timed-throughput-fixture.h"
#include "test/timed-throughput.h"
#include "util/parse-cmd-line.h"
#include "util/util.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

using mq::counted;
using std::string;
using std::vector;
using test::timed_throughput;
using test::timed_throughput_fixture;
using util::CmdLineOpts;
using util::makeLargeItem;
using util::parseCmdLineOpts;
using util::time::Time;

extern template class counted<vector<string>>;
extern template class counted<int>;

typedef timed_throughput_fixture<counted<vector<string>>, vector<string>,
                                 counted<int>, int> Fixture;

int main(int argc, char **argv) {
  CmdLineOpts opts;
  if (parseCmdLineOpts(argc, argv, opts) != 0)
    return 1;
  int max_num_threads = MAX(opts.num_producers, opts.num_consumers);

  counted<vector<string>> large_queue(max_num_threads);
  counted<int> small_queue(max_num_threads);

  vector<string> large_item;
  int small_item = 0;
  makeLargeItem(&large_item);

  Fixture fixture(&large_queue, &large_item, &small_queue, &small_item);
  if (fixture.run(argc, argv))
    return 0;
  else
    return 1;
}
