#include <cstdio>
#include <cstdlib>
#include "parse-cmd-line.h"

CmdLineOpts::CmdLineOpts()
    : num_producers(2)
    , num_consumers(2)
    , time_to_run(5)
    , use_large_test(false)
    , verbose(false) {}

void CmdLineOpts::describe() {
  fprintf(stderr, "%s items\n"
                  "%d consumers\n"
                  "%d producers\n"
                  "%d seconds to run\n",
          this->use_large_test ? "large" : "small", this->num_consumers,
          this->num_producers, this->time_to_run);
}

void printHelp() {
  fprintf(stderr,
      "Switches:\n"
      "\t-l       : use large item test (default false)\n"
      "\t-s       : use small item test (default true)\n"
      "\t-p <num> : specify number of producers (default 2)\n"
      "\t-c <num> : specify number of consumers (default 2)\n"
      "\t-t <num> : specify time to run in seconds (default 5)\n");
}

int parseCmdLineOpts(int argc, char **argv, CmdLineOpts &opts) {
  using namespace std;
  for (int i=1; i<argc; ++i) {
    if (argv[i][0] != '-') {
      fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
      return 1;
    }   
    char c = argv[i][1];

    if (c == 'l') {
      opts.use_large_test = true;
    } else if (c == 's') {
      opts.use_large_test = false;
    } else if (c == 'h') {
      printHelp();
      return 1;
    } else if (c == 'v') {
      opts.verbose = true;
    } else {
      if (c == 'p' || c == 'c' || c == 't') {
        ++i;
        if (i == argc) { // no more items
          fprintf(stderr, "Expected argument after switch '%c'\n", c);
          return 1;
        }

        if (c == 'p')
          opts.num_producers = atoi(argv[i]);
        else if (c == 'c')
          opts.num_consumers = atoi(argv[i]);
        else if (c == 't')
          opts.time_to_run = atoi(argv[i]);
      } else {
        fprintf(stderr, "Unrecognized switch '%c'\n", c);
        return 1;
      }
    }
  }
  return 0;
}
