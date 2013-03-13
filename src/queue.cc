#include "queue.h"
#include "util.h"

struct cvl::ms::Node
{
  public:
    int *data;
    Node *next;
    Node(int *data_, Node *next_) :
        data(data_), next(next_)
    {}
    ~Node()
    {
        delete data;
    }
};

cvl::ms::TwoLockQueue::TwoLockQueue()
{
    head = new Node(NULL, NULL);
    tail = head;
}

cvl::ms::TwoLockQueue::~TwoLockQueue()
{
    Node *del = head;
    Node *cur = del->next;
    while (cur != NULL)
    {
        delete del;
        del = cur;
        cur = cur->next;
    }
    delete del;
}

void cvl::ms::TwoLockQueue::enqueue(const int &data)
{
    Node *node = new Node(new int(data), NULL);

    tail_lock.lock();
    tail->next = node;
    tail = node;
    tail_lock.unlock();
}

bool cvl::ms::TwoLockQueue::dequeue(int &ret)
{
    head_lock.lock();
    Node *node = head;
    Node *new_head = head->next;
    if (new_head == NULL)
    {
        head_lock.unlock();
        return false;
    }
    ret  = *(new_head->data);
    head = new_head;
    head_lock.unlock();

    delete node;
    return true;
}

cvl::ms::LockFreeQueue::LockFreeQueue()
{
    head = new Node(NULL, NULL);
    tail = head;
}

cvl::ms::LockFreeQueue::~LockFreeQueue()
{
    Node *del = head;
    Node *cur = del->next;
    while (cur != NULL)
    {
        delete del;
        del = cur;
        cur = cur->next;
    }
    delete del;
}

void cvl::ms::LockFreeQueue::enqueue(const int &data)
{
    Node *mytail, *mynext;
    Node *node = new Node(new int(data), NULL);
    for (;;)
    {
        mytail = tail;
        mynext = mytail->next;
        if (mytail == tail)
        {
            if (mynext == NULL)
            {
                if (atomic::cas64((uint64_t *) &(tail->next), (uint64_t) mynext,
                                  (uint64_t) node))
                    break;
            }
            else
            {
                atomic::cas64((uint64_t *) &tail, (uint64_t) mytail,
                              (uint64_t) mynext);
            }
        }
    }
    atomic::cas64((uint64_t *) &tail, (uint64_t) mytail, (uint64_t) node);
}

bool cvl::ms::LockFreeQueue::dequeue(int &result)
{
    Node *myhead, *mytail, *mynext;
    for (;;)
    {
        myhead = head;
        mytail = tail;
        mynext = myhead->next;
        if (myhead == head)
        {
            if (myhead == mytail)
            {
                if (mynext == NULL)
                    return false;
                atomic::cas64((uint64_t *) &tail, (uint64_t) mytail, 
                              (uint64_t) mynext);
            }
            else
            {
                result = *(mynext->data);
                if (atomic::cas64((uint64_t *) &head, (uint64_t) myhead,
                                  (uint64_t) mynext))
                    break;
            }
        }
    }
    delete myhead;
    return true;
}

cvl::MultiQueue::MultiQueue(int num_queues_) :
    enqueue_cur(0), dequeue_cur(0), queues()
{
    num_queues = nextPow2(num_queues_);
    mask = num_queues-1;
    queues.reserve(num_queues);
    for (int i=0; i<num_queues; ++i)
        queues.push_back(new ms::TwoLockQueue());
}

cvl::MultiQueue::~MultiQueue()
{
    for (int i=0; i<num_queues; ++i)
        delete queues[i];
}

void cvl::MultiQueue::enqueue(const int &data)
{
    unsigned int mycur = atomic::fetchAndAdd(&enqueue_cur, 1);
    queues[mycur&mask]->enqueue(data);
}

bool cvl::MultiQueue::dequeue(int &ret)
{
    unsigned int mycur = atomic::fetchAndAdd(&dequeue_cur, 1);
    bool status;

    // This is bad as a consumer could be stuck here indefinately if the
    // producers decide not to enqueue anything, but I can't just return false
    // because then I'll be skipping over one of the items in the queue.
    //
    // Maybe this skipping would be desireable in some cases. Will have to work
    // out what that would mean for the overall functionality of the queue
    do
    {
        status = queues[mycur&mask]->dequeue(ret);
    } while (!status);
    return true;
}
