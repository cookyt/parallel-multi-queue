#ifndef TEST_TIMED_THROUGHPUT_H
#define TEST_TIMED_THROUGHPUT_H

#include <utility>
#include <unistd.h>
#include <boost/thread.hpp>

#include "util/atomic.h"
#include "util/time.h"

namespace test {

namespace {

// Boost thread worker functor. Takes a queue and repeatedly takes items from
// it. Stops when interrupted by a boost interruption. When that's done, each
// thread of this type aggregates the number of items it popped into a global
// value.
template <typename Queue, typename T>
class Consumer {
 private:
  Queue &Q;
  boost::barrier &bar;
  unsigned int local_throughput;
  volatile unsigned int *global_throughput;

 public:
  // Constructs a Consumer with 0 initial throughput.
  // @param Q The queue to take from.
  // @param bar_ Thread will wait on barrier before it starts its action.
  // @param global_throughput_ When it gets interrupted, thread will stop
  //   popping from the queue and add its counted throughput into the int
  //   pointed to by this variable..
  Consumer(Queue &Q_, boost::barrier &bar_,
           volatile unsigned int *global_throughput_)
      : Q(Q_)
      , bar(bar_)
      , local_throughput(0)
      , global_throughput(global_throughput_) {}

  // Copy constructor.
  Consumer(const Consumer &de)
      : Q(de.Q)
      , bar(de.bar)
      , local_throughput(de.local_throughput)
      , global_throughput(de.global_throughput) {}

  // Consumer action. Waits until all threads are instantiated and starts
  // popping things as fast as possible. Logs each successful pop
  // by incrementing a local counter.
  void operator()() {
    bar.wait();
    T val;
    try {
      for (;;) {
        boost::this_thread::interruption_point();
        if (Q.try_pop(val)) {
          // Incrementing throughput locally before adding it up
          // prevents the shared-counter contention problem but I
          // have to accept a certain degree of inaccuracy with
          // where the thread gets interrupted
          boost::this_thread::interruption_point();
          local_throughput++;
        }
      }
    } catch (boost::thread_interrupted &e) {
      // atomically add up all throughputs to get overall
      util::atomic::fetchAndAdd(global_throughput, local_throughput);
    }
  }
};

// Thread worker boost functor class. Takes a queue and repeatedly adds items
// into it until it's interrupted by a boost thread interruption.
template <typename Queue, typename T>
class Producer {
  private:
  Queue &Q;
  T item;
  boost::barrier &bar;

  public:
  // Creates a producer with a copy of the passed item, and a reference to the
  // queue and barrier.
  Producer(Queue &Q_, const T &item_, boost::barrier &bar_)
      : Q(Q_)
      , item(item_)
      , bar(bar_) {}

  // Copy constructor. Needed for boost threads.
  Producer(const Producer &eq)
      : Q(eq.Q)
      , item(eq.item)
      , bar(eq.bar) {}

  // Producer action. Waits until all threads are instantiated and starts
  // blindly enqueueing items like the devil's on it's heels
  void operator()() {
    bar.wait();
    for (;;) {
      boost::this_thread::interruption_point();
      Q.push(item);
    }
  }
};

}  // namespace

// Defines a basic test where a number of producers and an equal number of
// consumers are spawned and let loose to enqueue/dequeue one item each. The
// parameter is the type of queue you want to test. It must provide the
// following methods:
//   void push(const T &)
//   bool try_pop(T &)
template <typename Queue, typename T>
class timed_throughput {
 private:
  Queue &Q;
  int num_producers;
  int num_consumers;
  int time_limit;

 public:
  timed_throughput(Queue &Q_, int num_producers_, int num_consumers_,
                   int time_limit_)
      : Q(Q_)
      , num_producers(num_producers_)
      , num_consumers(num_consumers_)
      , time_limit(time_limit_) {}

  // Runs the simulation of producers and consumers for the given amount of
  // time.
  // @param item Each producer will get a copy of this item to "produce" in the
  //   queue
  // @return a pair of which corresponds to the time it took to run the test and
  //   the number of items through the queue.
  std::pair<util::time::Time, int> run(const T &item) {
    // the +1 allows a call to bar.wait() in this thread to start all
    // threads running. It's a way of makeing threads wait until all
    // threads are ready.
    boost::barrier bar(num_producers+num_consumers + 1);
    volatile unsigned int throughput = 0;

    boost::thread_group threads;
    for (int i=1; i<=num_producers; i++) {
      Producer<Queue, T> e(Q, item, bar);
      threads.create_thread(e);
    }
    for (int i=1; i<=num_consumers; i++) {
      Consumer<Queue, T> d(Q, bar, &throughput);
      threads.create_thread(d);
    }

    using util::time::Time;

    Time before = util::time::now_real();
    bar.wait(); // wait until all threads are ready
    sleep(time_limit);
    Time elapsed = util::time::now_real();
    threads.interrupt_all();

    // Wait for the consumers to finish tallying the throughput
    threads.join_all();

    elapsed -= before;
    return std::make_pair(elapsed, throughput);
  }
};

}  // namespace test

#endif // TEST_TIMED_THROUGHPUT_H
