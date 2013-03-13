Questions
---------
### What is the best way to measure throughput on a concurrent queue?
+ email Herb Sutter and ask about the rig he used for his Dr Dobbs article
  Measuring Parallel Performance
+ Read through some papers on concurrent queues and check the results sections.
  Maybe they provide source code.
+ Use a shared counter incremented by consumers and time the program. The
  contention on the shared counter would likely significantly affect run times,
  but a clever implementation might offset that effect.
+ Look up web site on dining philosophers which I used for as2. They had a
  testing rig for their programs which measured wait time on the threads.

### Where can I find a machine with many cores that I can test my code on?
+ Dr. Jha mentioned that UCF had a large multi-processor computer. If I ask, I
  might be able to get some time on that.


Namespaces
----------
### cvl
Top level namespace. My implementation of algorithms are here. My own
algorithms are in the top-level while the implemetations of other people's
algorithms are in sub-namespaces

### cvl::ms
My implementation of the concurrent queue algorithms defined in the paper
"Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue
Algorithms" by Micheal and Scott (hence the name ms). I based these of the
Pseudocode provided in the paper, but also applied some simple optimizations
akin to those described in the Dr Dobbs articles by Herb Stutter.


Resources
---------
### sutter 2-lock queue
+ http://www.drdobbs.com/parallel/writing-a-generalized-concurrent-queue/211601363?pgno=1

### measuring queues
+ http://www.drdobbs.com/parallel/measuring-parallel-performance-optimizin/212201163
