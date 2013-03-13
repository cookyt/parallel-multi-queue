#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <boost/thread/mutex.hpp>

/**
 * Top level namespace. If I come up with anything original, it's in the top of
 * this namespace.
 */
namespace cvl
{
    /**
     * Namespace stands for Michael & Scott, writers of the paper "Simple, Fast,
     * and Practical Non-Blocking and Blocking Concurrent Queue Algorithms" from
     * which the algorithms for these classes come.
     */
    namespace ms
    {
        class Node;

        /**
         * A two-lock concurrent queue. Based on a linked list, and uses two locks
         * for each the head and tail pointers.
         */
        class TwoLockQueue
        {
          private:
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

        class LockFreeQueue
        {
          private:
            Node *head;
            Node *tail;

          public:
            LockFreeQueue();
            ~LockFreeQueue();
            void enqueue(int);
            bool dequeue(int *);
        };
    }

    class MultiQueue
    {
      private:
        int num_queues;
        int mask;
        unsigned int volatile enqueue_cur;
        unsigned int volatile dequeue_cur;
        std::vector<ms::TwoLockQueue *> queues;

      public:
        MultiQueue(int);
        ~MultiQueue();
        void enqueue(int);
        bool dequeue(int *);
    };
}

#endif
