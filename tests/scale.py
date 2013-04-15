#!/usr/bin/env python

import sys
import hashlib

"""This script takes the output produced by logger.py, and scales the results
so that they can be passed into a grapher. It takes several files on the
cmdline, and produces an equal number of files, scaled to the global minimum
and maximum.

The scaling is between 0.01 and 0.5, and it is distributed so that the area of
a circle with the scaled value as a radius is proportional to the size of the
original value, globally.

The scaled files are saved in the 'scaled' directory, and have the same names
as the input files, prefixed with a SHA1 hash which is the same for all files
which were scaled together
"""

def script_dir():
    """Parses sys.argv[0] and gets the directory the script is in, including
    the trailing slash Returns the empty string on failure.
    """
    try:
        pos = sys.argv[0].rindex('/')
        return sys.argv[0][:pos]+"/"
    except ValueError:
        return ""

if __name__ == "__main__":
    min_rad = 0.01
    max_rad = 0.5
    d_rad = max_rad - min_rad

    max_val= 0.0
    min_val = float("inf")
    d_val = 0.0

    """Dictionary of file names to a list of 4 items:
        1. An open file pointer to the new scaled data file
        2. A list of 3-tuples with the old data: (consumer, producer, throughput)
        3. The min throughput for the above data
        4. The max throughput for the above data
        5. The max number of consumers
        6. The max number of producers
        7. The first line from the file (used as a title)
    """
    all_data = dict()

    # Short sha1 hash of all input values, to track files which have been scaled
    # together
    key = hashlib.sha1("".join(sys.argv[1:])).hexdigest()[:6]

    # Read all values into memory, find global max and min
    for fname in sys.argv[1:]:
        f = open(fname, "r")
        newname = script_dir()+"scaled/%s-%s"%(key,fname[fname.rfind('/')+1:])
        nf = open(newname, "w")
        all_data[fname] = [nf, list()]

        local_min = float("inf")
        local_max = 0.0
        max_consumers = 0.0
        max_producers = 0.0

        lines = f.readlines()
        for line in lines[1:]:
            if line[0] == "#":
                continue
            vals = line.split()

            consumers = int(vals[0])
            producers = int(vals[1])
            throughput = float(vals[2])

            max_val = max(throughput, max_val)
            min_val = min(throughput, min_val)

            local_max = max(throughput, local_max)
            local_min = min(throughput, local_min)

            max_consumers = max(consumers, max_consumers)
            max_producers = max(producers, max_producers)

            all_data[fname][1].append((consumers, producers, throughput))

        all_data[fname].append(local_min)
        all_data[fname].append(local_max)
        all_data[fname].append(max_consumers+1)
        all_data[fname].append(max_producers+1)
        all_data[fname].append(lines[0].replace("#", "").strip()) # title

        f.close()

    d_val = max_val - min_val

    def scale(val):
        """Returns a scaling of the given value"""
        d_rad2 = d_rad*d_rad
        min_rad2 = min_rad*min_rad
        return (((val-min_val)/d_val)*d_rad2 + min_rad2)**0.5

    def scale_lin(val):
        return (((val-min_val)/d_val)*d_rad + min_rad)

    for (fname, data) in all_data.iteritems():
        nf = data[0]

        nf.write("#!/usr/bin/env gnuplot\n")
        nf.write("# min=%f, max=%f\n"%(data[2], data[3]))
        nf.write("set title '%s'\n"%data[6])
        nf.write("set size ratio -1\n")
        nf.write("set xrange [0:%d]\n"%data[4])
        nf.write("set yrange [0:%d]\n"%data[5])
        nf.write("set xlabel 'consumers'\n")
        nf.write("set ylabel 'producers'\n")
        nf.write("plot '-' with circles fill solid notitle\n")
        for point in data[1]:
            nf.write("%d\t%d\t%f\n"%(point[0], point[1], scale(point[2])))
        nf.write("e\n")
        nf.write("pause -1")

        nf.close()
