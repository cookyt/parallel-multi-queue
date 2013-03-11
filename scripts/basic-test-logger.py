#!/usr/bin/env python
import os

""" This script appends to a log file called `log` the results of several time
trials of a list of binaries and their averages. Each binary must take, as a
cmdline arg, the number of threads it spawns. It also gets the short SHA-1 hash
of the current git branch and prepends it to the log of each binary """

log = open("log", "a")

""" The number of times a given binary is tested """
num_execs = 10

""" The number of threads to pass to each binary """
num_threads = 10000

""" The list of binaries to test """
bins = ["bin/locking-test", "bin/multi-test", "bin/lock-free-test"]

build_cmd = os.popen("git rev-parse --short HEAD")
build = build_cmd.read().strip();

def test(exec_name):
    """Tests a single executable num_exec number of times, and logs each result
    (and their average) to the log file. It uses the system command
    `time --format %e` to time the executables"""
    full_name = "%s %d"%(exec_name, num_threads)
    log.write("%s %s\n"%(build, full_name))
    avg = 0.0
    for i in range(num_execs):
        cmd = os.popen3("time --format %e "+full_name)
        time = cmd[2].read()
        log.write("\t%s"%time)
        avg += float(time)
    log.write("AVG %f\n\n"%(avg/float(num_execs)))

for binary in bins:
    test(binary)
