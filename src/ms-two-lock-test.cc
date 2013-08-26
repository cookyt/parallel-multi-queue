#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

#include "queue.h"
#include "test/timed-throughput-fixture.h"
#include "test/timed-throughput.h"
#include "util/util.h"

using ms::two_lock;
using std::string;
using std::vector;
using test::timed_throughput;
using test::timed_throughput_fixture;
using util::makeLargeItem;
using util::time::Time;

extern template class two_lock<int>;
extern template class two_lock<vector<string>>;

typedef timed_throughput_fixture<two_lock<vector<string>>, vector<string>,
                                 two_lock<int>, int> Fixture;

int main(int argc, char **argv) {
  two_lock<vector<string>> large_queue;
  two_lock<int> small_queue;

  int small_item = 0;
  vector<string> large_item;
  makeLargeItem(&large_item);

  Fixture fixture(&large_queue, &large_item, &small_queue, &small_item);
  if (fixture.run(argc, argv))
    return 0;
  else
    return 1;
}
