#ifndef QUEUE_MQ_COUNTED_H
#define QUEUE_MQ_COUNTED_H

#include <vector>
#include <boost/thread.hpp>

#include "queue/ms/two-lock.h"
#include "util/atomic.h"
#include "util/util.h"

namespace mq {

using util::atomic::cas;

/**
 * Experimental MultiQueue. It uses the a two-lock queue developed by
 * Micheal and Scott internally. Currently, operations create a full copy
 * of the item via the copy constructor and pass around pointers to the
 * object in the queues themselves.
 */
template<typename T, typename Q = ms::two_lock<T>>
class counted {
 private:
  int num_queues;
  int mask;
  unsigned int volatile enqueue_cur;
  unsigned int volatile dequeue_cur;

  // includes padding to avoid false sharing
  struct PaddedQueue {
    Q queue;
    char padding[util::kCacheLineSize];
  };

  // Outside of constructor, only lookup operations are called.
  std::vector<PaddedQueue *> queues;

 public:
  counted(int num_queues_) :
      enqueue_cur(0), dequeue_cur(0) {
    num_queues = util::nextPow2(num_queues_);
    mask = num_queues-1;
    queues.reserve(num_queues);
    for (int i=0; i<num_queues; ++i)
      queues.push_back(new PaddedQueue());
  }

  ~counted() {
    for (int i=0; i<num_queues; ++i)
      delete queues[i];
  }

  bool push(const T &item) {
    unsigned int mycur = util::atomic::fetchAndAdd(&enqueue_cur, 1);
    return queues[mycur&mask]->queue.push(item);
  }

  bool pop(T &result) {
    unsigned int mycur;
    for (;;) {
      mycur = dequeue_cur;
      if (mycur >= enqueue_cur)
        return false;

      // TODO(cookyt): this shouldn't be using the GCC-specific implementation
      // of CAS. Replace it with C++11 atomics
      if (cas(&dequeue_cur, mycur, mycur+1))
        break;
    }

    // I don't like having this loop here, without it, a consumer may
    // see an empty queue when in reality, the producer hasn't finished
    // yet. This might allow the cur item pointer to skip over an item
    // in the queue, making the non-linearizable
    while (!queues[mycur&mask]->queue.pop(result)) {
      // If there are producers, a consumer might get stuck here
      // forever. I need a way out.
      boost::this_thread::interruption_point();
    }
    return true;
  }
};

}  // namespace mq

#endif // QUEUE_MQ_COUNTED_H
