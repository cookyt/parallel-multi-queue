MicroQueues
===========

Description of a MultiQueue
---------------------------
The concepts surrounding the MultiQueue are simple enough. The structure keeps
an internal list of concurrent queues and two counters that keep track how many
enqueue and dequeue operations have been made on the MultiQueue. In essence, the
internal list is treated like a ring buffer of queues, and the two counters
essentially point to the head and tail of this queue. The intent is to isolate
the enqueue and dequeue operations of the more complex internal concurrent queue
algorithms so that, on average, each queue has only one producer and one
consumer operating on it. Because of the ordering enforced by the MultiQueue,
all the queues in the ring buffer constitute a single large queue, so,
externally, the interface to the MultiQueue remains the same as the interface to
a regular concurrent queue.

Previous Work
-------------
The idea for the MultiQueue is not novel. Below is an overview of several
researchers who have published this idea before, and how their approach differs
from the above.

### Distributed Queues in Shared Memory
Authors: Anreas Haas et. al.
Published: May 2013, ACM 978-1-4503-2053-5

Shows two k-linearizable implementations of a queue using a similar approach as
MQ. One implementation uses an LRU algorithm with ABA counters in the head and
tail of each internal queue which gets updated with a DCAS. The other uses a
load balancer. K is very high for this one. The round robin had good
performance, but not significantly above that of other methods (particularly the
k-linearizable chained block queue described in the quasi-linearizability
paper). The load-balanced one got several orders better performance than the
competition (particularly, it was better than a concurrent Bag algorithm).

As an aside: The researchers behind this paper have been working on relaxed
concurrency modles (in particular, k-relaxed queues) for the last few years, and
released a paper which acts as a direct predecessor to the load-balanced version
of the algorithm presented in this paper. The title of this paper is `Fast and
Scalable k-fifo queues` by Kirsch et. al.


### Multilane Multisets
Authors: Dave Dice, Oleksandr Otenko
Published: Brief announcement @ SPAA 2011

This is only an announcement of the idea, not a full paper. It talks about
approch using multiple "lanes" instead of queues, and describes that the
algorithm can be used to compose several blocking containers (not strictly FIFO)
of the same type into a collection of containers which exhibit the same public
API and guarantees, but is much more scalable. Claims that their approach does
not guarantee strict FIFO ordering even if internal container is FIFO.


### Intel TBB's Micro Queues
+ Forum discussion on implementation
  <http://software.intel.com/en-us/forums/topic/294768>
+ Description of how `tbb::concurrent_queue` works
  <http://blogs.msdn.com/b/nativeconcurrency/archive/2010/01/16/concurrent-vector-and-concurrent-queue-explained.aspx>

Although not an accademically published work, it's worth mentioning that TBB
uses a similar approach which it calls a micro queue. It fixes the number of
internal queues to eight and uses a chained-block linked list queue internally.
One odd thing about it is that it accesses the internal queues in an odd order.
The ith enqueued element is put into internal queue `(i*3) % 8`.

I can't find any published work about micro queues. Most things I've found are
message board discussions amongst developers and blog posts explaining them with
some detail.  That said, some of the posts date back to 2008~2009, so it looks
like the idea has been around for a while.

Possible Future Work
--------------------
The most configurable thing about the approach is the algorithm to choose an
internal queue. One promising idea is to write an algorithm which guarantees
some interesting ordering property of the elements in the queue (ex:
quasi-linearizability). The distributed queues paper took this approach, and its
algorithm for load-balanced choosing between queues had excellent performance
against other pool-like datastructures.

One possiblity is to go in the opposite direction, and try to create a
weight-free queue using this which guarantees FIFO ordering. The multiset paper
claimed that their approach could not guarante FIFO, so there is a bit of
novelty in giving that guarantee.
