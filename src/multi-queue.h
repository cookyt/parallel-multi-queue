#include "pthread.h"

// class Queue
// {
//   public:
//     bool empty();
//     int  size();
//     int  front();
//     int  back();
//     void push(int);
//     void pop();
// };

class LockingQueue
{
  private:
    class Item
    {
      public:
        int data;
        Item *next;
        Item(int, Item *);
    };
    Item *first;
    Item *last;
    int count;
    pthread_mutex_t lock;

  public:
    LockingQueue();
    bool empty();
    int  size();
    void push(int);
    int  pop();
};

class MultiQueue
{
  private:
    LockingQueue *queues;
    int count;
    int cur;
  public:
    MultiQueue(int);
};
