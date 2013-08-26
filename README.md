Parallel MultiQueue
===================
This is a study of trying to improve the concurrency of a concurrent queue
structure. The idea revolves around keeping several queue structures in a ring
buffer, and trying to give each thread access to exactly one at a time. The
hope is that with multiple threads working on different queues, contention
between threads is lessened.

Directory Layout
----------------
+ bin/
  Compiled code (linkable objects and executable binaries)

+ doc/
  Documentation used to research topic

+ paper/
  Latex source for papers

+ src/
  Source code

+ tests/
  Testing scripts and data from various test runs

Build Information
---------------------
### Dependancies
+ Only tested on 64 bit Linux, may not work elsewhere
+ Relies on librealtime (Linux specific, and maybe BSD) to accuately time tests
+ Only compiles with GCC as it uses GCC's wrappers to x86 atomic primatives
  (compiled with version 4.6)
+ Needs libboost-thread

### Building and Running
Just run `make` to compile the source. This should generate some executable
binaries in the `bin/` directory which correspond to testing binaries for a
two-lock queue and a multiqueue. Each executable runs a test which measures the
throughput of a queue under load from a given number of producer and consumer
threads. They accept the following flags:

    -l       : use large item test (default false)
    -s       : use small item test (default true)
    -p <num> : specify number of producers (default 2)
    -c <num> : specify number of consumers (default 2)
    -t <num> : specify time to run in seconds (default 5)
    -h       : prints the help text
    -v       : verbose

The default behavior is to run the test and print to stdout the throughput of
the queue in items per second.

### Testing Scripts
The `tests` directory contains two Python scripts which automatically tests an
executable under various thread configurations.

`logger.py` takes three arguments:
1. Name of an executable from the `bin/` directory
2. Number of consumer threads to test up to
3. Number of producer threads to test up to

It will generate a data file in the `tests/data/` directory. The file is named
for the datetime the script was run at.

`scale.py` takes a variable number of data files produced by `logger.py` and
produces an equal number of gnuplot scripts which, when run, generate an SVG
file which graph the throughput of each passed file. The generated graphs are
scaled to the maximum of all logged values in the passed file arguments. The
scripts are generated in the `tests/scaled/` directory.

License
-------
Copyright (c) 2013 Carlos Valera

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
