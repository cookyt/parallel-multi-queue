#ifndef BASIC_TEST_H
#define BASIC_TEST_H

#include <utility>
#include <unistd.h>
#include <boost/thread.hpp>
#include "util.h"

// Defines a basic test where a number of producers and an
// equal number of consumers are spawned and let loose to
// enqueue/dequeue one item each. The parameter is the type of
// queue you want to test. It must provide the following
// methods:
//   void enqueue(const T &)
//   bool dequeue(T &)
template <typename Queue, typename T>
class BasicTest
{
  private:
    Queue &Q;
    int num_producers;
    int num_consumers;
    int time_limit;

    class Consumer
    {
      private:
        Queue &Q;
        boost::barrier &bar;
        unsigned int local_throughput;
        volatile unsigned int *global_throughput;

      public:
        Consumer(Queue &Q_, boost::barrier &bar_, volatile unsigned int *global_throughput_) :
            Q(Q_), bar(bar_), local_throughput(0),
            global_throughput(global_throughput_)
        {}

        Consumer(Consumer &de) :
            Q(de.Q), bar(de.bar), local_throughput(de.local_throughput),
            global_throughput(de.global_throughput)
        {}

        /**
         * Consumer action. Waits until all threads are instantiated and starts
         * dequeuing things as fast as possible. Logs each successful dequeue
         * by incrementing a shared counter.
         */
        void operator()()
        {
            bar.wait();
            T val;
            bool status;
            try
            {
                for (;;)
                {
                    boost::this_thread::interruption_point();
                    if (Q.dequeue(val))
                    {
                        // Incrementing throughput locally before adding it up
                        // prevents the shared-counter contention problem but I
                        // have to accept a certain degree of inaccuracy with
                        // where the thread gets interrupted
                        boost::this_thread::interruption_point();
                        local_throughput++;
                    }
                }
            }
            catch (boost::thread_interrupted &e)
            {
                // atomically add up all throughputs to get overall
                cvl::atomic::fetchAndAdd(global_throughput, local_throughput);
            }
        }
    };

    class Producer
    {
      private:
        Queue &Q;
        T item;
        boost::barrier &bar;

      public:
        /**
         * Creates a producer with a copy of the passed item, and a reference to the queue and barrier.
         */
        Producer(Queue &Q_, const T &item_, boost::barrier &bar_) :
            Q(Q_), item(item_), bar(bar_)
        {}

        /**
         * Copy constructor. Needed for boost threads.
         */
        Producer(Producer &eq) :
            Q(eq.Q), item(eq.item), bar(eq.bar)
        {}

        /**
         * Producer action. Waits until all threads are instantiated and starts
         * blindly enqueueing items like the devil's on it's heels
         */
        void operator()()
        {
            bar.wait();
            for (;;)
            {
                boost::this_thread::interruption_point();
                Q.enqueue(item);
            }
        }
    };
  public:
    BasicTest(Queue &Q_, int num_producers_, int num_consumers_, int time_limit_) :
        Q(Q_), num_producers(num_producers_), num_consumers(num_consumers_), time_limit(time_limit_)
    {}

    /**
     * Runs the simulation of producers and consumers for the given amount of
     * time.
     * @param item Each producer will get a copy of this item to "produce" in the queue
     * @return a pair of 
     */
    std::pair<cvl::time::Time, int> run(const T &item)
    {
        // the +1 allows a call to bar.wait() in this thread to start all
        // threads running. It's a way of makeing threads wait until all
        // threads are ready.
        boost::barrier bar(num_producers+num_consumers + 1);
        volatile unsigned int throughput = 0;

        boost::thread_group threads;
        for (int i=1; i<=num_producers; i++)
        {
            Producer e(Q, item, bar);
            threads.create_thread(e);
        }
        for (int i=1; i<=num_consumers; i++)
        {
            Consumer d(Q, bar, &throughput);
            threads.create_thread(d);
        }

        using cvl::time::Time;

        Time before = cvl::time::now_real();
        bar.wait();
        sleep(time_limit);
        Time elapsed = cvl::time::now_real();
        threads.interrupt_all();

        // Wait for the consumers to finish tallying the throughput
        threads.join_all();

        elapsed -= before;
        return std::make_pair(elapsed, throughput);
    }
};

#endif
