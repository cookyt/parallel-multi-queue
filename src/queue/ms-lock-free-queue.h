#ifndef MS_LOCK_FREE_QUEUE_H
#define MS_LOCK_FREE_QUEUE_H

#include "util.h"

namespace cvl
{
    namespace ms
    {
        template<typename T>
        class LockFreeQueue
        {
          private:
            struct Node
            {
                Node(T *data_, Node *next_) : data(data_), next(next_) {}
                ~Node ()
                {
                    delete data;
                }
                T *data;
                Node *next;
            };

            Node *head;
            Node *tail;

          public:
            LockFreeQueue()
            {
                head = new Node(NULL, NULL);
                tail = head;
            }

            ~LockFreeQueue()
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

            void enqueue(const T &item)
            {
                Node *mytail, *mynext;
                Node *node = new Node(new T(item), NULL);
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

            bool dequeue(T &result)
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
        };
    }
}

#endif
