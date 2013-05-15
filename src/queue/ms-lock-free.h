#ifndef MS_LOCK_FREE_H
#define MS_LOCK_FREE_H

#include <cstdio>
#include <cstddef> // used to define NULL
#include "util.h"

namespace ms
{
    template<typename T>
    class lock_free
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
        lock_free()
        {
            head = new Node(NULL, NULL);
            tail = head;
        }

        ~lock_free()
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

        void push(const T &item)
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
                        if (util::atomic::cas(&(mytail->next), mynext, node))
                            break;
                    }
                    else
                    {
                        util::atomic::cas(&tail, mytail, mynext);
                    }
                }
            }
            util::atomic::cas(&tail, mytail, node);
        }

        bool try_pop(T &result)
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
                        util::atomic::cas(&tail, mytail, mynext);
                    }
                    else
                    {
                        result = *(mynext->data);
                        if (util::atomic::cas(&head, myhead, mynext))
                            break;
                    }
                }
            }
            delete myhead;
            return true;
        }
    };
}

#endif // MS_LOCK_FREE_H
