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
    int  dequeue();
};

class MultiQueue
{
  private:
    LockingQueue *queues;
    int cur;
    int num_queues;
  public:
    MultiQueue(int);
};
