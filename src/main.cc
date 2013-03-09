#include <iostream>
#include "multi-queue.h"

int main()
{
    LockingQueue q;
    for (int i=0; i<10; i++)
        q.enqueue(i);

    for (int i=0; i<10; i++)
        std::cout << q.dequeue() << std::endl;
    return 0;
}
