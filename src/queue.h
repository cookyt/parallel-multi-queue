#ifndef MULTI_QUEUE_H
#define MULTI_QUEUE_H

#include <vector>
#include <boost/thread/mutex.hpp>

/**
 * Namespace stands for Michael Scott, writers of the paper "Simple, Fast, and
 * Practical Non-Blocking and Blocking Concurrent Queue Algorithms" from which
 * the algorithms for these classes come.
 */
namespace ms
{
    /**
     * A two-lock concurrent queue. Based on a linked list, and uses two locks
     * for each the head and tail pointers.
     */
    class TwoLockQueue
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
        TwoLockQueue();
        ~TwoLockQueue();
        void enqueue(int);
        bool dequeue(int *);
    };
}

/**
 * If I come up with anything original, it's in this namespace.
 */
namespace cvl
{
    class MultiQueue
    {
      private:
        int num_queues;
        int mask;
        volatile unsigned int enqueue_cur;
        volatile unsigned int dequeue_cur;
        std::vector<ms::TwoLockQueue *> queues;
      public:
        MultiQueue(int);
        ~MultiQueue();
        void enqueue(int);
        bool dequeue(int *);
    };
}

#endif
