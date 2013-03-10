#ifndef BASIC_TEST_H
#define BASIC_TEST_H

# if DEBUG
#  include <cstdio>
# endif
#include <boost/thread.hpp>

// Defines a basic test where a number of producers and an
// equal number of consumers are spawned and let loose to
// enqueue/dequeue one item each. The parameter is the type of
// queue you want to test. It must provide the following
// methods:
//   void enqueue(int)
//   bool dequeue(int *)
template <typename Queue>
class BasicTest
{
  private:
    Queue &Q;
    int num_threads;
    class Consumer
    {
      private:
        Queue &Q;
        int id;
        boost::barrier &bar;

      public:
        Consumer(Queue &Q_, int id_, boost::barrier &bar_) :
            Q(Q_), id(id_), bar(bar_)
        {}

        Consumer(Consumer &de) :
            Q(de.Q), id(de.id), bar(de.bar)
        {}

        void operator()()
        {
            bar.wait();
            int val;
            bool status;
            do
            {
                status = Q.dequeue(&val);
            } while (!status);
# if DEBUG
            std::fprintf(stderr, "%d ", val);
# endif
        }
    };

    class Producer
    {
      private:
        Queue &Q;
        int id;
        boost::barrier &bar;

      public:
        Producer(Queue &Q_, int id_, boost::barrier &bar_) :
            Q(Q_), id(id_), bar(bar_)
        {}

        Producer(Producer &eq) :
            Q(eq.Q), id(eq.id), bar(eq.bar)
        {}

        void operator()()
        {
            bar.wait();
            Q.enqueue(id);
        }
    };
  public:
    BasicTest(Queue &Q_, int num_threads_) :
        Q(Q_), num_threads(num_threads_)
    {}

    void run()
    {
        boost::barrier bar(2*num_threads);
        boost::thread_group threads;
        for (int i=1; i<=num_threads; i++)
        {
            Producer e(Q, i, bar);
            Consumer d(Q, num_threads+i, bar);
            threads.create_thread(e);
            threads.create_thread(d);
        }
        threads.join_all();
# if DEBUG
        std::fputs("\n", stderr);
# endif
    }
};

#endif
