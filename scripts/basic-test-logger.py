#!/usr/bin/env python
import os
import subprocess as sp

""" This script appends to a log file called `log` the results of several time
trials of a list of binaries and their averages. Each binary must take, as a
cmdline arg, the number of threads it spawns. It also gets the short SHA-1 hash
of the current git branch and prepends it to the log of each binary """

fname = "scripts/log/basic-test-log"

""" The number of times a given binary is tested """
num_execs = 1

""" The number of threads to pass to each binary """
num_threads = 1

""" The list of binaries to test """
bins = ["bin/locking-test", "bin/multi-test", "bin/lock-free-test"]

class Logger:
    def __init__(self, fname, num_execs, num_threads):
        self.build = self.get_build()
        self.log = self.open_log(fname)
        self.cpu_name = self.get_cpu_name()
        self.num_execs = num_execs
        self.num_threads = num_threads

    def get_build(self):
        """Gets the SHA-1 hash of the current GIT branch"""
        build_cmd = os.popen("git rev-parse --short HEAD")
        return build_cmd.read().strip();

    def get_cpu_name(self):
        """Tries to get the name of the CPU. It returns a string with either
        the name of the CPU or a message saying it could not get the name. Only
        works on Linux"""
        error_msg = "Could not determine CPU name\n\n"
        try:
            # Spawning a `sed` for this is probably bad practice. It's quicker
            # to write, but I should use python-internal stuff instead.
            cmd = sp.Popen(['sed', '-n', r'/model name/{ s/model name\s*: //; p; q 0 }', '/proc/cpuinfo'], stdout=sp.PIPE)
            cpu_name = cmd.communicate()[0]
            if (cmd.returncode == 0):
                cpu_name = cpu_name+"\n"
            else:
                cpu_name = error_msg
        except OSError:
            cpu_name = error_msg
        return cpu_name

    def open_log(self, fname):
        """Opens the log file for appending data. If the log is initially
        empty, it tries to get the name of the CPU and writes that as the first
        line in the log"""
        if os.stat(fname).st_size == 0:
            log = open(fname, "a")
            log.write(self.get_cpu_name())
        else:
            log = open(fname, "a")
        return log

    def test(self, exec_name):
        """Tests a single executable num_exec number of times, and logs each result
        (and their average) to the log file. It uses the system command
        `time --format %e` to time the executables"""
        full_name = "%s %d"%(exec_name, self.num_threads)
        self.log.write("%s %s\n"%(self.build, full_name))

        avg = 0.0
        bad_tests = 0
        tests = list()
        for i in range(self.num_execs):
            cmd = os.popen3("time --format %e "+full_name)
            time = cmd[2].read()
            try:
                avg += float(time)
                tests.append("\t%s"%time)
            except ValueError:
                sys.stderr.write("Encountered a problem with a test on %s, recorded time is %s\n"%(full_name, time));
                bad_tests += 1

        self.log.write("AVG %f %d\n"%(avg/float(self.num_execs-bad_tests), self.num_execs-bad_tests))
        for test in tests:
            self.log.write(test)
        self.log.write('\n')

if __name__ == "__main__":
    logger = Logger(fname, num_execs, num_threads)
    for binary in bins:
        logger.test(binary)
