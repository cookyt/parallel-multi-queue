DEBUG=0
OBJ = obj/util.o \
      obj/parse-cmd-line.o \
      obj/mq-counted.o \
      obj/ms-lock-free.o \
      obj/ms-two-lock.o \
      obj/test-mq-counted.o \
      obj/test-ms-lock-free.o \
      obj/test-ms-two-lock.o

BIN = bin/mq-counted-test \
      bin/ms-two-lock-test \
      bin/ms-lock-free-test

INC = -Isrc/
CXXFLAGS = -std=c++0x -g -Wall -pthread -DDEBUG=$(DEBUG)

LIBBOOST = -lboost_thread -lboost_system
LIB = $(LIBBOOST) -lrt

.PHONY: all
all: $(BIN)

# Binaries
bin/mq-counted-test: \
		src/mq-counted-test.cc \
		obj/mq-counted.o \
		obj/parse-cmd-line.o \
		obj/test-mq-counted.o \
		obj/time.o \
		obj/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

# TODO(cookyt): implement
bin/bit-network-test: \
		src/mq-bit-network.cc \
		obj/ms-two-lock.o \
		obj/parse-cmd-line.o \
		obj/test-ms-two-lock.o \
		obj/time.o \
		obj/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/ms-two-lock-test: \
		src/ms-two-lock-test.cc \
		obj/ms-two-lock.o \
		obj/parse-cmd-line.o \
		obj/test-ms-two-lock.o \
		obj/time.o \
		obj/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/ms-lock-free-test: \
		src/ms-lock-free-test.cc \
		obj/ms-lock-free.o \
		obj/parse-cmd-line.o \
		obj/test-ms-lock-free.o \
		obj/time.o \
		obj/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

# Libraries
obj/parse-cmd-line.o: \
		src/util/parse-cmd-line.cc \
		src/util/parse-cmd-line.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/util.o: \
		src/util/util.cc \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/time.o: \
		src/util/time.cc \
		src/util/time.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/ms-two-lock.o: \
		src/templ/ms/two-lock.cc \
		src/queue/ms/two-lock.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/ms-lock-free.o: \
		src/templ/ms/lock-free.cc \
		src/queue/ms/lock-free.h \
		src/util/atomic.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/mq-counted.o: \
		src/templ/mq/counted.cc \
		src/queue/mq/counted.h \
		src/util/atomic.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/test-ms-two-lock.o: \
		src/templ/test/ms-two-lock.cc \
		src/test/timed-throughput.h \
		src/queue/ms/two-lock.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/test-ms-lock-free.o: \
		src/templ/test/ms-lock-free.cc \
		src/test/timed-throughput.h \
		src/queue/ms/lock-free.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/test-mq-counted.o: \
		src/templ/test/mq-counted.cc \
		src/test/timed-throughput.h \
		src/queue/mq/counted.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

.PHONY: clean-obj
clean-obj:
	-$(RM) $(OBJ)

.PHONY: clean-bin
clean-bin:
	-$(RM) $(BIN)

.PHONY: clean-tags
clean-tags:
	-$(RM) */tags */types*.taghl

.PHONY: clean-all
clean-all: clean-obj clean-tags clean-bin

.PHONY: clean
clean: clean-obj clean-bin
