#ifndef MULTI_QUEUE_H
#define MULTI_QUEUE_H

#include <vector>
#include <boost/thread/mutex.hpp>

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
    boost::mutex head_lock;
    boost::mutex tail_lock;

  public:
    LockingQueue();
    ~LockingQueue();
    void enqueue(int);
    bool dequeue(int *);
};

class MultiQueue
{
  private:
    int num_queues;
    int mask;
    volatile unsigned int enqueue_cur;
    volatile unsigned int dequeue_cur;
    std::vector<LockingQueue *> queues;
  public:
    MultiQueue(int);
    ~MultiQueue();
    void enqueue(int);
    bool dequeue(int *);
};

#endif
