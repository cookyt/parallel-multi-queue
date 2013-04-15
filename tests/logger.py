#!/usr/bin/env python
import os
import sys
import subprocess as sp
import datetime as dt

"""
This script performs various tests on the given queue binary executable. It
expects three command line arguments:
    The name of executable
    The maximum number of consumer threads to test
    The maximum number of producer threads to test
    (optional) any arguments to pass to the executable

Further, the executable should implement the following switches
    -c <num> for the number of consumer threads
    -p <num> for the number of producer threads

And should, when run, print a single floating point number representing the
throughput of the queue in items per second.

The script generates a file 'data/yyyymmdd:HHMM.dat' (local time)
This file has three columns:
    1. number of consumer threads tested
    2. number of producer threads tested
    3. the throughput of the test (items/sec)
"""

class Logger:
    def __init__(self, logname, exe_name, consumers, producers):
        self.exe_name = exe_name
        self.consumers = consumers
        self.producers = producers
        self.log = self.open_log(logname)
        self.data = list()

    def get_build(self):
        """Gets the SHA-1 hash of the current git branch"""
        build_cmd = os.popen("git rev-parse --short HEAD")
        ret = build_cmd.read().strip();
        if ret == "":
            return "Could not get Git branch"
        else:
            return ret

    def get_cpu_name(self):
        """Tries to get the name of the CPU. It returns a string with either
        the name of the CPU or a message saying it could not get the name. Uses
        the proc/ directory, so it only works on Linux"""

        error_msg = "Could not determine CPU name"
        try:
            # Spawning a `sed` for this is probably bad practice. It's quicker
            # to write, but I should use python-internal stuff instead.
            cmd = sp.Popen(['sed', '-n', r'/model name/{ s/model name\s*: //; p; q 0 }', '/proc/cpuinfo'], stdout=sp.PIPE)
            cpu_name = cmd.communicate()[0]
            if (cmd.returncode == 0):
                cpu_name = cpu_name.replace("\n", "")
            else:
                cpu_name = error_msg
        except OSError:
            cpu_name = error_msg
        return cpu_name

    def open_log(self, logname):
        """Opens the log file with a header specifiying the executable being
        tested, the current git branch, and the name of the CPU"""
        log = open(logname, "a")
        log.write("# test for: %s\n"%self.exe_name)
        log.write("# build: %s\n"%self.get_build())
        log.write("# CPU: %s\n"%self.get_cpu_name())
        return log

    def run(self):
        """Tests the executable for each configuration of consumers and
        producers, and logs the result to the log file."""

        for cnum in range(1, self.consumers+1):
            for pnum in range(1, self.producers+1):
                print "trying: consumers=%d, producers=%d"%(cnum, pnum)
                cmdpipe = os.popen("%s -c %d -p %d"%(self.exe_name, cnum, pnum))
                time_str = cmdpipe.read()
                try:
                    time = float(time_str)
                    self.data.append((cnum, pnum, time));
                except ValueError:
                    sys.stderr.write("Error with trial:\n"
                                     "\tconsumers=%d\n"
                                     "\tproducers=%d\n"
                                     "\toutput is %s\n"%(cnum, pnum, time_str))

        for item in self.data:
            self.log.write("%d\t%d\t%f\n"%item)


if __name__ == "__main__":
    if len(sys.argv) < 4:
        sys.stderr.write("Insufficient arguments\n")
        sys.stderr.write("need: executable consumers producers\n")
        raise SystemExit

    exe_name = sys.argv[1]
    consumers = int(sys.argv[2])
    producers = int(sys.argv[3])

    if consumers < 1 or producers < 1:
        sys.stderr.write("producers and num consumers must be at least 1\n")
        raise SystemExit

    time = dt.datetime.now()
    logname = "tests/data/%s.dat"%(time.strftime("%Y%m%d:%H%M"));

    logger = Logger(logname, exe_name, consumers, producers)
    logger.run()
