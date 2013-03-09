#include "multi-queue.h"

LockingQueue::Node::Node(int data_, Node *next_) :
    data(data_), next(next_)
{}

LockingQueue::LockingQueue()
{
    pthread_mutex_init(&head_lock, NULL);
    pthread_mutex_init(&tail_lock, NULL);
    head = new Node(0, NULL);
    tail = head;
}

void LockingQueue::enqueue(int data)
{
    Node *node = new Node(data, NULL);

    pthread_mutex_lock(&tail_lock);
    tail->next = node;
    tail = node;
    pthread_mutex_unlock(&tail_lock);
}

int LockingQueue::dequeue()
{
    pthread_mutex_lock(&head_lock);
    Node *node = head;
    Node *new_head = head->next;
    if (new_head == NULL)
    {
        pthread_mutex_unlock(&head_lock);
        return 0;
    }
    int data = new_head->data;
    head = new_head;
    pthread_mutex_unlock(&head_lock);

    delete node;
    return data;
}

MultiQueue::MultiQueue(int size_) :
    _size(size_), cur(0)
{
    queues = new LockingQueue[_size];
}
