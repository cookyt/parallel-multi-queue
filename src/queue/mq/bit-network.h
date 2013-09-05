#ifndef QUEUE_MQ_BIT_NETWORK_H
#define QUEUE_MQ_BIT_NETWORK_H

#include <atomic>
#include <vector>
#include <stdint.h>

namespace queue {
namespace mq {
namespace {

uint_fast32_t log2(unsigned w) {
  static const uint_fast32_t  b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
  static const uint_fast32_t S[] = {1, 2, 4, 8, 16};

  uint_fast32_t r = 0;
  for (int_fast8_t i = 4; i >= 0; --i) {
    if (w & b[i]) {
      w >>= S[i]; 
      r |= S[i]; 
    } 
  }
  return (r);
}

}  // namespace

template <typename T, typename Q>
class bit_network {
  typedef std::vector<std::atomic_int> atomic_int_vector_t;
  typedef std::vector<Q> queue_vector_t;

 private:
  atomic_int_vector_t  enq_network_;
  atomic_int_vector_t  deq_network_;
  queue_vector_t     buckets_;

 public:
  bit_network(int num_queues)
      : enq_network_(1U<<log2(num_queues))
      , deq_network_(1U<<log2(num_queues))
      , buckets_(1U<<log2(num_queues)) {}

  ~bit_network() {}

  inline void push(const T& x) {
    auto idx = select_queue_index(enq_network_);
    buckets_[idx].push(x);
  }

  inline bool pop(T& x) {
    unsigned idx = select_queue_index(deq_network_);
    unsigned const n = buckets_.size();
    for (unsigned i = 0; i < n; ++i) {
      // try all to dequeue from all n queues until giving up.
      // start trying from the selected index.
      if (buckets_[(idx+i)&(n-1)].pop(x)) {
        return (true);
      }
    }
    return (false);
  }

  private:
  unsigned select_queue_index(atomic_int_vector_t& vec) {
    static const unsigned n = buckets_.size();
    unsigned bit, idx = 1;
    do {
      auto& atomic_bit = enq_network_[idx];
      bit = atomic_bit++; // same as atomic::fetch_add(1)
    } while ((idx = (idx<<1) + ((bit&1) ? 1 : 0)) < n);
    return (idx-n);
  }
};

}  // namespace mq
}  // namespace queue

#endif  // QUEUE_MQ_BIT_NETWORK_H
