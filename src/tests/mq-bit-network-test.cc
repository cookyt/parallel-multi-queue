#include "../queue/mq-bit-network.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <queue>
#include <mutex>
#include <thread>

namespace mq 
{
    template <typename T>
    class naive : public std::queue<T> 
    {
      private:
        std::mutex mutex_;
    
      public:
        naive()
        {}

        explicit naive(const naive<T>& other)
        {}

        ~naive()
        {}

        void push(const T& x) {
            std::unique_lock<std::mutex> lock(mutex_);
            std::queue<T>::push(x);
        }

        bool try_pop(T& x)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!this->empty()) {
                x = this->front();
                this->pop();
                return (true);
            }
            return (false);
        }
    };
}

static const unsigned num_queues = 1U<<3;
static const unsigned ops_per_thread = 1U<<10;
static const unsigned num_pushers = 1u<<2;
static const unsigned num_poppers = 1u<<2;
static const unsigned num_threads = num_pushers + num_poppers;

typedef int value_t;
typedef mq::naive<value_t> queue_t;
typedef mq::bit_network<value_t, queue_t> bit_network_queue_t;

static void single_threaded_test(bit_network_queue_t& q)
{
    for (unsigned i=0; i<ops_per_thread; ++i) {
        q.push(i);
    }
    int x;
    for (unsigned i=0; i<ops_per_thread; ++i) {
        if (q.try_pop(x)) {
            //std::cout << x << ' ';
        }
    }
    //std::cout << std::endl;
}

static void multi_threaded_test(bit_network_queue_t& q)
{
    std::vector<std::thread> threads(num_threads);

    for (auto& t : threads) {
        t = std::thread(single_threaded_test, std::ref(q));
    }
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main(int argc, char** argv)
{
    bit_network_queue_t q(num_queues);
    
    if (argc > 1) {
        switch (atoi(argv[1])) {
            case 1:
                single_threaded_test(q);
                break;
            case 2:
                multi_threaded_test(q);
                break;
        }
    } 
    else {
        // default test:
        multi_threaded_test(q);
    }
    return (0);
}
