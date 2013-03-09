#include "multi-queue.h"

LockingQueue::Node::Node(int data_, Node *next_) :
    data(data_), next(next_)
{}

LockingQueue::LockingQueue()
{
    head = new Node(0, NULL);
    tail = head;
}

LockingQueue::~LockingQueue()
{
    delete head;
}

void LockingQueue::enqueue(int data)
{
    Node *node = new Node(data, NULL);

    tail_lock.lock();
    tail->next = node;
    tail = node;
    tail_lock.unlock();
}

int LockingQueue::dequeue()
{
    head_lock.lock();
    Node *node = head;
    Node *new_head = head->next;
    if (new_head == NULL)
    {
        head_lock.unlock();
        return 0;
    }
    int data = new_head->data;
    head = new_head;
    head_lock.unlock();

    delete node;
    return data;
}

MultiQueue::MultiQueue(int num_queues_) :
    num_queues(num_queues_), cur(0)
{
    queues = new LockingQueue[num_queues_];
}
