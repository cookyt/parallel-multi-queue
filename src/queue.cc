#include "queue.h"

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

bool LockingQueue::dequeue(int *ret)
{
    head_lock.lock();
    Node *node = head;
    Node *new_head = head->next;
    if (new_head == NULL)
    {
        head_lock.unlock();
        return false;
    }
    *ret = new_head->data;
    head = new_head;
    head_lock.unlock();

    delete node;
    return true;
}

int nextPow2(int n)
{
    int cur = 1;
    while (cur < n)
        cur = cur << 1;
    return cur;
}

MultiQueue::MultiQueue(int num_queues_) :
    enqueue_cur(0), dequeue_cur(0), queues()
{
    num_queues = nextPow2(num_queues_);
    mask = num_queues-1;
    queues.reserve(num_queues);
    for (int i=0; i<num_queues; ++i)
        queues[i] = new LockingQueue();
}

MultiQueue::~MultiQueue()
{
    for (int i=0; i<num_queues; ++i)
        delete queues[i];
}

void MultiQueue::enqueue(int data)
{
    // This is a builtin function for GCC. Should replace it with a more
    // portable method, but it's good for now for testing.
    unsigned int mycur = __sync_fetch_and_add(&enqueue_cur, 1);

    queues[mycur&mask]->enqueue(data);
}

bool MultiQueue::dequeue(int *ret)
{
    // This is a builtin function for GCC. Should replace it with a more
    // portable method, but it's good for now for testing.
    unsigned int mycur = __sync_fetch_and_add(&dequeue_cur, 1);

    bool status;
    do
    {
        status = queues[mycur&mask]->dequeue(ret);
    } while (!status);
    return true;
}
