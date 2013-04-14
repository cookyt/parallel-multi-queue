#ifndef MS_LOCK_FREE_QUEUE_H
#define MS_LOCK_FREE_QUEUE_H

#include <cstdio>
#include <cstddef> // used to define NULL
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
                            if (atomic::cas(&(mytail->next), mynext, node))
                                break;
                        }
                        else
                        {
                            atomic::cas(&tail, mytail, mynext);
                        }
                    }
                }
                atomic::cas(&tail, mytail, node);
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
                            atomic::cas(&tail, mytail, mynext);
                        }
                        else
                        {
                            result = *(mynext->data);
                            if (atomic::cas(&head, myhead, mynext))
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
