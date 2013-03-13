#ifndef MULTI_QUEUE_H
#define MULTI_QUEUE_H

#include <vector>
#include "queue/ms-two-lock-queue.h"
#include "util.h"

namespace cvl
{
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
            unsigned int mycur = atomic::fetchAndAdd(&dequeue_cur, 1);
            bool status;

            // This is bad as a consumer could be stuck here indefinately if the
            // producers decide not to enqueue anything, but I can't just return false
            // because then I'll be skipping over one of the items in the queue.
            //
            // Maybe this skipping would be desireable in some cases. Will have to work
            // out what that would mean for the overall functionality of the queue
            do
            {
                status = queues[mycur&mask]->dequeue(result);
            } while (!status);
            return true;
        }
    };
}

#endif
