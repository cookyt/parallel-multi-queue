#ifndef BASIC_TEST_H
#define BASIC_TEST_H

#include <unistd.h>
#include <boost/thread.hpp>
#include "util.h"

// Defines a basic test where a number of producers and an
// equal number of consumers are spawned and let loose to
// enqueue/dequeue one item each. The parameter is the type of
// queue you want to test. It must provide the following
// methods:
//   void enqueue(const int &)
//   bool dequeue(int &)
template <typename Queue, typename T>
class BasicTest
{
  private:
    Queue &Q;
    int num_producers;
    int num_consumers;

    class Consumer
    {
      private:
        Queue &Q;
        boost::barrier &bar;

      public:
        Consumer(Queue &Q_, boost::barrier &bar_) :
            Q(Q_), bar(bar_)
        {}

        Consumer(Consumer &de) :
            Q(de.Q), bar(de.bar)
        {}

        void operator()()
        {
            bar.wait();
            T val;
            bool status;
            for (;;)
            {
                if (Q.dequeue(val))
                {
                    // TODO log consumed item
                }
                boost::this_thread::interruption_point();
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
        Producer(Queue &Q_, const T &item_, boost::barrier &bar_) :
            Q(Q_), item(item_), bar(bar_)
        {}

        Producer(Producer &eq) :
            Q(eq.Q), item(eq.item), bar(eq.bar)
        {}

        void operator()()
        {
            bar.wait();
            for (;;)
            {
                Q.enqueue(item);
                boost::this_thread::interruption_point();
            }
        }
    };
  public:
    BasicTest(Queue &Q_, int num_producers_, int num_consumers_) :
        Q(Q_), num_producers(num_producers_), num_consumers(num_consumers_)
    {}

    /**
     * Runs the simulation of producers and consumers for the given amount of
     * time.
     * @param secs The simulation will run for at least this number of seconds
     * @param item Each producer will get a copy of this item to "produce" in the queue
     */
    void run(int secs, const T &item)
    {
        boost::barrier bar(num_producers+num_consumers);
        boost::thread_group threads;
        for (int i=1; i<=num_producers; i++)
        {
            Producer e(Q, item, bar);
            threads.create_thread(e);
        }
        for (int i=1; i<=num_consumers-1; i++)
        {
            Consumer d(Q, bar);
            threads.create_thread(d);
        }

        using cvl::time::Time;

        Time before = cvl::time::now_cpu();
        Consumer d(Q, bar);
        threads.create_thread(d);
        sleep(secs);
        threads.interrupt_all();
        Time elapsed = cvl::time::now_cpu() - before;
    }
};

#endif
