#!/usr/bin/env python

import sys

"""This script takes the output produced by logger.py, and scales the results
so that they can be passed into a grapher. It takes several files on the
cmdline, and produces an equal number of files, scaled to the global minimum
and maximum.

The scaling is between 0.01 and 0.5, and it is distributed so that the area of
a circle with the scaled value as a radius is proportional to the size of the
original value, globally.
"""

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
"""
all_data = dict()

# Read all values into memory, find global max and min
for fname in sys.argv[1:]:
    f = open(fname, "r")
    nf = open("%s-scaled"%fname, "w")
    all_data[fname] = [nf, list()]
    local_min = float("inf")
    local_max = 0.0
    for line in f.readlines():
        if line[0] == "#": # Copy comments to new file, and skip
            nf.write(line)
            continue
        vals = line.split()

        throughput = float(vals[2])
        max_val = max(throughput, max_val)
        min_val = min(throughput, min_val)
        local_max = max(throughput, local_max)
        local_min = min(throughput, local_min)

        all_data[fname][1].append((int(vals[0]), int(vals[1]), throughput))
    all_data[fname].append(local_min)
    all_data[fname].append(local_max)
    f.close()

d_val = max_val - min_val

def scale(val):
    """Returns a scaling of the given value"""
    d_rad2 = d_rad*d_rad
    min_rad2 = min_rad*min_rad
    return (((val-min_val)/d_val)*d_rad2 + min_rad2)**0.5

for (fname, data) in all_data.iteritems():
    nf = data[0]
    nf.write("# min=%f, max=%f\n"%(data[2], data[3]))
    for point in data[1]:
        nf.write("%d\t%d\t%f\n"%(point[0], point[1], scale(point[2])))
    nf.close()
