#ifndef TEST_TIMED_THROUGHPUT_FIXTURE_H
#define TEST_TIMED_THROUGHPUT_FIXTURE_H

#include <stdio.h>
#include <stdlib.h>

#include <utility>

#include "test/timed-throughput.h"
#include "util/parse-cmd-line.h"

namespace test {

template<typename LargeItemQueue, typename LargeItem, typename SmallItemQueue,
         typename SmallItem>
class timed_throughput_fixture {
 public:

  // Pointers do not belong to this object. Uses the respective queue in the
  // test, and copies the respective item into the test.
  timed_throughput_fixture(LargeItemQueue *large_queue,
                           LargeItem *large_item,
                           SmallItemQueue *small_queue,
                           SmallItem *small_item)
      : large_queue_(large_queue)
      , large_item_(large_item)
      , small_queue_(small_queue)
      , small_item_(small_item) {}

  // Runs the test.
  bool run(int argc, char **argv);

 private:
  typedef timed_throughput<LargeItemQueue, LargeItem> LargeTest;
  typedef timed_throughput<SmallItemQueue, SmallItem> SmallTest;

  // Chooses the correct test to run (large or small) based on opts, and runs
  // the timed test. Returns the results of the test.
  std::pair<util::time::Time, int> runTest(const util::CmdLineOpts &opts);

  // Prints. to stdout, the results of a test run. Checks the opts to see
  // whether verbose output should be printed.
  void printResults(const util::CmdLineOpts &opts,
                    const std::pair<util::time::Time, int> &results);

  LargeItemQueue *large_queue_;
  LargeItem *large_item_;
  SmallItemQueue *small_queue_;
  SmallItem *small_item_;
};

template<typename LargeItemQueue, typename LargeItem, typename SmallItemQueue,
         typename SmallItem>
std::pair<util::time::Time, int> timed_throughput_fixture
    <LargeItemQueue, LargeItem, SmallItemQueue, SmallItem>::runTest(
    const util::CmdLineOpts &opts) {
  using util::time::Time;

  std::pair<Time, int> throughput;
  if (opts.use_large_test) {
    LargeTest test(*large_queue_, opts.num_producers, opts.num_consumers,
                   opts.time_to_run);
    throughput = test.run(*large_item_);
  } else {
    SmallTest test(*small_queue_, opts.num_producers, opts.num_consumers,
                   opts.time_to_run);
    throughput = test.run(*small_item_);
  }

  return throughput;
}

template<typename LargeItemQueue, typename LargeItem, typename SmallItemQueue,
         typename SmallItem>
void timed_throughput_fixture
    <LargeItemQueue, LargeItem, SmallItemQueue, SmallItem>::printResults(
    const util::CmdLineOpts &opts,
    const std::pair<util::time::Time, int> &results) {
  int items = results.second;
  double time = ((double) results.first.secs) +
                ((double) results.first.nsecs)/1e9;

  if (opts.verbose)
    printf("throughput: %lf items/sec\n", items/time);
  else
    printf("%lf\n", items/time);
}

template<typename LargeItemQueue, typename LargeItem, typename SmallItemQueue,
         typename SmallItem>
bool timed_throughput_fixture
    <LargeItemQueue, LargeItem, SmallItemQueue, SmallItem>::run(int argc,
                                                                char **argv) {
  using util::CmdLineOpts;
  using util::time::Time;

  CmdLineOpts opts;
  if (util::parseCmdLineOpts(argc, argv, opts) != 0)
    return false;
  if (opts.verbose)
    opts.describe();

  std::pair<Time, int> results = runTest(opts);
  printResults(opts, results);
  return true;
}

}  // namespace test

#endif // TEST_TIMED_THROUGHPUT_FIXTURE_H
