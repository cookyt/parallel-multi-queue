#ifndef MULTI_QUEUE_H
#define MULTI_QUEUE_H

#include <vector>
#include "queue/ms-two-lock-queue.h"
#include "util.h"

namespace cvl
{
    /** 
     * Experimental MultiQueue. It uses the a two-lock queue developed by
     * Micheal and Scott internally. Currently, operations create a full copy
     * of the item via the copy constructor and pass around pointers to the
     * object in the queues themselves.
     */
    template<typename T>
    class MultiQueue
    {
      private:
        int num_queues;
        int mask;
        unsigned int volatile enqueue_cur;
        unsigned int volatile dequeue_cur;
        std::vector<ms::TwoLockQueue<T> *> queues;

      public:
        MultiQueue(int num_queues_) : 
            enqueue_cur(0), dequeue_cur(0)
        {
            num_queues = nextPow2(num_queues_);
            mask = num_queues-1;
            queues.reserve(num_queues);
            for (int i=0; i<num_queues; ++i)
                queues.push_back(new ms::TwoLockQueue<T>());
        }

        ~MultiQueue()
        {
            for (int i=0; i<num_queues; ++i)
                delete queues[i];
        }

        void enqueue(const T &item)
        {
            unsigned int mycur = atomic::fetchAndAdd(&enqueue_cur, 1);
            queues[mycur&mask]->enqueue(item);
        }

        bool dequeue(T &result)
        {
            // Causes a problem if the integer overflows.
            if (dequeue_cur >= enqueue_cur) // Make sure there are items left to dequeue
                return false;

            unsigned int mycur = atomic::fetchAndAdd(&dequeue_cur, 1); // Gain exclusivity to a queue

            // I don't like having this loop here, but it makes sense to have it.
            while (!queues[mycur&mask]->dequeue(result)) {}
            return true;
        }
    };
}

#endif
