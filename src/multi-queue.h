#include "pthread.h"

class MultiQueue
{
  private:
    class LockingQueue
    {
      private:
        class item
        {
          public:
            int data;
            item *next;
        };
        item *front;
        item *last;
        pthread_mutex_t lock;
    };
    LockingQueue *queues;
    int size;
  public:
    MultiQueue(int);
};
