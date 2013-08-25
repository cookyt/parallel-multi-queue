#ifndef MS_TWO_LOCK_H
#define MS_TWO_LOCK_H

#include <cstddef> // used to define NULL
#include <boost/thread/mutex.hpp>
#include "util.h"

namespace ms {

/**
 * A two-lock concurrent queue. It's based on a linked list and uses
 * two locks for each of the head and tail pointers. The template
 * parameter must provide the copy constructor and the assignment
 * operator. This version allocates memory for new nodes, and new
 * copies of the passed item.
 */
template<typename T>
class two_lock {
 private:
  struct Node {
    Node(T *data_, Node *next_) : data(data_), next(next_) {}
    ~Node () {
      delete data;
    }

    T *data;
    Node *next;
    char padding[util::kCacheLineSize];
  };

  Node *head;
  Node *tail;
  boost::mutex head_lock;
  boost::mutex tail_lock;

 public:
  two_lock() {
    head = new Node(NULL, NULL);
    tail = head;
  }

  ~two_lock() {
    Node *del = head;
    Node *cur = del->next;
    while (cur != NULL) {
      delete del;
      del = cur;
      cur = cur->next;
    }
    delete del;
  }

  /**
   * Enqueues an item into the queue.
   * @param item The item to push. This item is copied into the
   *   queue, so the copy constructor must be defined on T.
   */
  void push(const T &item) {
    // Constructing the node outside the critical section allows
    // for some concurrency between producers.
    Node *node = new Node(new T(item), NULL);

    tail_lock.lock();
    tail->next = node;
    tail = node;
    tail_lock.unlock();
  }

  /**
   * Dequeues an item from the queue.
   * @param result [out] where to store the dequeued item. This is
   *   done via assignment, so operator= must be defined on T.
   * @return true if an item was dequeued, false if the queue was
   *   empty.
   */
  bool try_pop(T &result) {
    head_lock.lock();
    Node *node = head;
    Node *new_head = head->next;
    if (new_head == NULL) {
      head_lock.unlock();
      return false;
    }

    // Copy back value, we're still in the critical section, so
    // copying here will slow things down significantly, but the
    // algorithm requires it (otherwise the node and its value
    // might be freed by another thread calling try_pop)
    result = *(new_head->data);
    head = new_head;
    head_lock.unlock();

    // Node destructor also handles deleting stored pointer
    delete node;
    return true;
  }
};

}  // namespace ms

#endif // MS_TWO_LOCK_H
