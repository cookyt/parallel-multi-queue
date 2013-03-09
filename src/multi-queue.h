#include "pthread.h"

/**
 * A two-lock concurrent queue based on the Michael and Scott paper:
 * "Simple, Fast, and Practical Non-Blocking and Blocking Concurrent
 * Queue Algorithms".
 */
class LockingQueue
{
  private:
    class Node
    {
      public:
        int data;
        Node *next;
        Node(int, Node *);
    };
    Node *head;
    Node *tail;
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;

  public:
    LockingQueue();
    void enqueue(int);
    int  dequeue();
};

class MultiQueue
{
  private:
    LockingQueue *queues;
    int cur;
    int _size;
  public:
    MultiQueue(int);
};
