#include <cstdlib>
#include <cstdio>

#include <string>
#include <vector>

#include "queue.h"
#include "test/timed-throughput-fixture.h"
#include "test/timed-throughput.h"
#include "util/util.h"

using ms::lock_free;
using std::string;
using std::vector;
using test::timed_throughput;
using test::timed_throughput_fixture;
using util::makeLargeItem;
using util::time::Time;

extern template class lock_free<int>;
extern template class lock_free<vector<string>>;

typedef timed_throughput_fixture<lock_free<vector<string>>, vector<string>,
                                 lock_free<int>, int> Fixture;

int main(int argc, char **argv) {
  lock_free<vector<string>> large_queue;
  lock_free<int> small_queue;

  int small_item = 0;
  vector<string> large_item;
  makeLargeItem(&large_item);

  Fixture fixture(&large_queue, &large_item, &small_queue, &small_item);
  if (fixture.run(argc, argv))
    return 0;
  else
    return 1;
}
