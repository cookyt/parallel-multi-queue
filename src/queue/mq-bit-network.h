#ifndef MQ_BIT_NETWORK_H
#define MQ_BIT_NETWORK_H

#include <atomic>
#include <vector>
#include "util.h"

namespace mq 
{
    template <typename T, typename Q>
    class bit_network 
    {
      private:
        struct node 
        {
            std::atomic_bool bit;
            char padding[cvl::kCacheLineSize];
        };
        
        std::vector<node> network_;
        std::vector<Q<T>> queues_;
    
      public:
        bit_network(int num_queues): 
            network_(cvl::nextPow2(num_queues))
            queues_(cvl::nextPow2(num_queues))
        {}

        ~bit_network()
        {}
    };
}

#endif // MQ_BIT_NETWORK_H
