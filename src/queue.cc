#include "queue.h"

ms::TwoLockQueue::Node::Node(int data_, Node *next_) :
    data(data_), next(next_)
{}

ms::TwoLockQueue::TwoLockQueue()
{
    head = new Node(0, NULL);
    tail = head;
}

ms::TwoLockQueue::~TwoLockQueue()
{
    delete head;
}

void ms::TwoLockQueue::enqueue(int data)
{
    Node *node = new Node(data, NULL);

    tail_lock.lock();
    tail->next = node;
    tail = node;
    tail_lock.unlock();
}

bool ms::TwoLockQueue::dequeue(int *ret)
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

cvl::MultiQueue::MultiQueue(int num_queues_) :
    enqueue_cur(0), dequeue_cur(0), queues()
{
    num_queues = nextPow2(num_queues_);
    mask = num_queues-1;
    queues.reserve(num_queues);
    for (int i=0; i<num_queues; ++i)
        queues[i] = new ms::TwoLockQueue();
}

cvl::MultiQueue::~MultiQueue()
{
    for (int i=0; i<num_queues; ++i)
        delete queues[i];
}

/**
 * This adds the given value to the word at the specified location and returns
 * the value prior to the addition. This is based on a builtin function for
 * GCC. Eventually, it should be replaced with a more portable implementation,
 * but for now, it's good for testing.
 */
unsigned int cvl::MultiQueue::fetchAndAdd(unsigned int volatile *addr, unsigned int val)
{
    return __sync_fetch_and_add(addr, val);
}

void cvl::MultiQueue::enqueue(int data)
{
    unsigned int mycur = fetchAndAdd(&enqueue_cur, 1);
    queues[mycur&mask]->enqueue(data);
}

bool cvl::MultiQueue::dequeue(int *ret)
{
    unsigned int mycur = fetchAndAdd(&dequeue_cur, 1);
    bool status;
    do
    {
        status = queues[mycur&mask]->dequeue(ret);
    } while (!status);
    return true;
}
