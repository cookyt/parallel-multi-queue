#include <cstdio>
#include <vector>
#include <boost/thread.hpp>
#include <unistd.h>
#include "multi-queue.h"

int const kNumThreads = 100;

class dequeuer
{
  private:
    LockingQueue &Q;
    int id;
    boost::barrier &bar;

  public:
    dequeuer(LockingQueue &Q_, int id_, boost::barrier &bar_) :
        Q(Q_), id(id_), bar(bar_)
    {}

    dequeuer(dequeuer &de) :
        Q(de.Q), id(de.id), bar(de.bar)
    {}

    void operator()()
    {
        bar.wait();
        int val;
        do
        {
            val = Q.dequeue();
        } while (val == 0);
        printf("%d ", val);
    }
};

class enqueuer
{
  private:
    LockingQueue &Q;
    int id;
    boost::barrier &bar;

  public:
    enqueuer(LockingQueue &Q_, int id_, boost::barrier &bar_) :
        Q(Q_), id(id_), bar(bar_)
    {}

    enqueuer(enqueuer &eq) :
        Q(eq.Q), id(eq.id), bar(eq.bar)
    {}

    void operator()()
    {
        bar.wait();
        Q.enqueue(id);
    }
};

int main()
{
    LockingQueue q;
    boost::barrier bar(2*kNumThreads);

    boost::thread_group threads;
    for (int i=1; i<=kNumThreads; i++)
    {
        enqueuer e(q, i, bar);
        dequeuer d(q, kNumThreads+i, bar);
        threads.create_thread(e);
        threads.create_thread(d);
    }
    threads.join_all();
    printf("\n");

    return 0;
}
