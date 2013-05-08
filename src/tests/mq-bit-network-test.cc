#include "../queue/mq-bit-network.h"
#include <iostream>
#include <queue>
#include <mutex>

namespace mq 
{
    template <typename T>
    class naive : std::queue<T> 
    {
      private:
        std::mutex mutex_;
    
      public:
        void push(const T& x) {
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

int main()
{
    mq::bit_network<int, mq::naive<int>> q(8);
    for (unsigned i=0; i<8; ++i) {
        q.push(i);
    }
    int x;
    for (unsigned i=0; i<8; ++i) {
        if (q.try_pop(x)) {
            std::cout << x << ' ';
        }
    }
    std::cout << std::endl;
    return (0);
}
