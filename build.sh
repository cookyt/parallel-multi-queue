#
# This file exists because the current Makefile tries to include boost, which is now
# deprecated and not supported by the mq namespace. Go ahead and remove this once the
# makefile is changed not to include boost.
#
# USAGE: build.sh <filename>
#        -- Do not pass the file extension, just the base name.
#        -- For example, "myfile", not "myfile.cc"

CC=g++
OPTIMIZATION=''
FLAGS='-std=c++11 -g -Wall -pthread'
LIBS='-lpthread'
DEFS='-Dthread_local=__thread'
BIN_DIR='bin'

eval "$CC $FLAGS $DEFS $OPTIMIZATION $1.cc -o $BIN_DIR/${1##*/}.out"
