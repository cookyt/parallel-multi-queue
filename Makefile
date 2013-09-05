DEBUG=0
OBJ = build/templ/mq/counted.o \
      build/templ/ms/lock-free.o \
      build/templ/ms/two-lock.o \
      build/templ/test/mq-counted.o \
      build/templ/test/ms-lock-free.o \
      build/templ/test/ms-two-lock.o \
      build/util/parse-cmd-line.o \
      build/util/time.o \
      build/util/util.o

BIN = build/mq-counted-test \
      build/ms-two-lock-test \
      build/ms-lock-free-test \
      build/boost-lockfree-queue-timed-test

# sort removes duplicates for a cleaner-looking mkdir command
EXPECTED_DIRS = $(sort $(dir $(OBJ)))

INC = -Isrc/
CXXFLAGS = -std=c++0x -g -Wall -pthread -DDEBUG=$(DEBUG)

LIBBOOST = -lboost_thread -lboost_system
LIB = -L/usr/local/lib $(LIBBOOST) -lrt

.PHONY: all
all: $(EXPECTED_DIRS) $(BIN)

$(EXPECTED_DIRS): %:
	mkdir -p $@

# Binaries
build/boost-lockfree-queue-timed-test: \
		src/boost-lockfree-queue-timed-test.cc \
		build/util/parse-cmd-line.o \
		build/util/time.o \
		build/util/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

build/mq-counted-test: \
		src/mq-counted-test.cc \
		build/templ/mq/counted.o \
		build/templ/test/mq-counted.o \
		build/util/parse-cmd-line.o \
		build/util/time.o \
		build/util/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

# TODO(cookyt): implement
build/bit-network-test: \
		src/mq-bit-network.cc \
		build/templ/ms/two-lock.o \
		build/templ/test/ms-two-lock.o \
		build/util/parse-cmd-line.o \
		build/util/time.o \
		build/util/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

build/ms-two-lock-test: \
		src/ms-two-lock-test.cc \
		build/templ/ms/two-lock.o \
		build/templ/test/ms-two-lock.o \
		build/util/parse-cmd-line.o \
		build/util/time.o \
		build/util/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

build/ms-lock-free-test: \
		src/ms-lock-free-test.cc \
		build/templ/ms/lock-free.o \
		build/templ/test/ms-lock-free.o \
		build/util/parse-cmd-line.o \
		build/util/time.o \
		build/util/util.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

# Libraries
build/util/parse-cmd-line.o: \
		src/util/parse-cmd-line.cc \
		src/util/parse-cmd-line.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/util/util.o: \
		src/util/util.cc \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/util/time.o: \
		src/util/time.cc \
		src/util/time.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/templ/ms/two-lock.o: \
		src/templ/ms/two-lock.cc \
		src/queue/ms/two-lock.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/templ/ms/lock-free.o: \
		src/templ/ms/lock-free.cc \
		src/queue/ms/lock-free.h \
		src/util/atomic.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/templ/mq/counted.o: \
		src/templ/mq/counted.cc \
		src/queue/mq/counted.h \
		src/util/atomic.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/templ/test/ms-two-lock.o: \
		src/templ/test/ms-two-lock.cc \
		src/test/timed-throughput.h \
		src/queue/ms/two-lock.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/templ/test/ms-lock-free.o: \
		src/templ/test/ms-lock-free.cc \
		src/test/timed-throughput.h \
		src/queue/ms/lock-free.h \
		src/util/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

build/templ/test/mq-counted.o: \
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
