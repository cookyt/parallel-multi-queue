#ifndef QUEUE_MS_LOCK_FREE_H
#define QUEUE_MS_LOCK_FREE_H

#include "util/atomic.h"

namespace ms {

using util::atomic::cas;

template<typename T>
class lock_free {
 private:
  struct Node {
    Node(T *data_, Node *next_) : data(data_), next(next_) {}

    ~Node () {
      delete data;
    }

    T *data;
    Node *next;
  };

  Node *head;
  Node *tail;

 public:
  lock_free() {
    head = new Node(nullptr, nullptr);
    tail = head;
  }

  ~lock_free() {
    Node *del = head;
    Node *cur = del->next;
    while (cur != nullptr) {
      delete del;
      del = cur;
      cur = cur->next;
    }
    delete del;
  }

  bool push(const T &item) {
    Node *mytail, *mynext;
    Node *node = new Node(new T(item), nullptr);
    for (;;) {
      mytail = tail;
      mynext = mytail->next;
      if (mytail == tail) {
        if (mynext == nullptr) {
          if (cas(&(mytail->next), mynext, node))
            break;
        } else {
          cas(&tail, mytail, mynext);
        }
      }
    }
    cas(&tail, mytail, node);
    return true;
  }

  bool pop(T &result) {
    Node *myhead, *mytail, *mynext;
    for (;;) {
      myhead = head;
      mytail = tail;
      mynext = myhead->next;
      if (myhead == head) {
        if (myhead == mytail) {
          if (mynext == nullptr) {
            return false;
          }
          cas(&tail, mytail, mynext);
        } else {
          result = *(mynext->data);
          if (cas(&head, myhead, mynext))
            break;
        }
      }
    }
    delete myhead;
    return true;
  }
};

}  // namespace ms

#endif  // QUEUE_MS_LOCK_FREE_H
