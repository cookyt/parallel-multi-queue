#include "multi-queue.h"

LockingQueue::Item::Item(int data_, Item *next_) :
    data(data_), next(next_)
{}

LockingQueue::LockingQueue() :
    count(0), first(NULL), last(NULL)
{
    pthread_mutex_init(&lock, NULL);
}

bool LockingQueue::empty()
{
    return (count == 0);
}

int LockingQueue::size()
{
    return count;
}

void LockingQueue::push(int data)
{
    pthread_mutex_lock(&lock);

    Item *pushed = new Item(data, NULL);
    if (last == NULL)
        first = pushed;
    else
        last->next = pushed;
    last = pushed;

    count += 1;

    pthread_mutex_unlock(&lock);
}

int LockingQueue::pop()
{
    pthread_mutex_lock(&lock);

    Item *popped = first;
    first = first->next;
    if (first == NULL)
        last = NULL;
    int data = popped->data;
    delete popped;
    count -= 1;

    pthread_mutex_unlock(&lock);

    return data;
}

MultiQueue::MultiQueue(int count_) :
    count(count_), cur(0)
{
    queues = new LockingQueue[count];
}
