#ifndef PARSE_CMD_LINE_H
#define PARSE_CMD_LINE_H

/**
 * Encases several run-time options for running tests.
 */
struct CmdLineOpts {
  /**
   * Initializes with the following defaults:
   *   num_producers  = 2
   *   num_consumers  = 2
   *   time_to_run    = 5
   *   use_large_test = false
   *   verbose        = false
   */
  CmdLineOpts();

  /**
   * Prints a message describing the current state of the options to stderr.
   */
  void describe();

  int num_producers;
  int num_consumers;
  int time_to_run;
  bool use_large_test;
  bool verbose;
};

/**
 * Parses command line options. Puts output into the opts reference struct.
 * Returns 0 on success, non-zero on failure
 *
 * Switches:
 *   -l       : use large item test (default false)
 *   -s       : use small item test (default true)
 *   -p <num> : specify number of producers (default 2)
 *   -c <num> : specify number of consumers (default 2)
 *   -t <num> : specify time to run in seconds (default 5)
 */
int parseCmdLineOpts(int, char **, CmdLineOpts &);

#endif
