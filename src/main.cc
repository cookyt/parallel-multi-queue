#include <iostream>
#include "multi-queue.h"

int main()
{
    LockingQueue q;
    for (int i=0; i<10; i++)
        q.push(i);

    for (int i=0; i<10; i++)
        std::cout << q.pop() << std::endl;
    return 0;
}
