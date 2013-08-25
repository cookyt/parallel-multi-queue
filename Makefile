DEBUG=0
OBJ = obj/util.o \
      obj/parse-cmd-line.o \
      obj/mq-counted.o \
      obj/ms-lock-free.o \
      obj/ms-two-lock.o \
      obj/test-mq-counted.o \
      obj/test-ms-lock-free.o \
      obj/test-ms-two-lock.o

BIN = bin/locking-test \
      bin/multi-test \
      bin/lock-free-test

INC = -Isrc/
CXXFLAGS = -std=c++0x -g -Wall -pthread -DDEBUG=$(DEBUG)

LIBBOOST = -lboost_thread -lboost_system
LIB = $(LIBBOOST) -lrt

all: $(BIN)

bin/multi-test: \
		src/test/mq-counted.cc \
		obj/mq-counted.o \
		obj/test-mq-counted.o \
		obj/util.o \
		obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/bit-network-test: \
		src/test/mq-bit-network.cc \
		obj/test-ms-two-lock.o \
		obj/ms-two-lock.o \
		obj/util.o \
		obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/locking-test: \
		src/test/ms-two-lock.cc \
		obj/test-ms-two-lock.o \
		obj/ms-two-lock.o \
		obj/util.o \
		obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/lock-free-test: \
		src/test/ms-lock-free.cc \
		obj/test-ms-lock-free.o \
		obj/ms-lock-free.o \
		obj/util.o \
		obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

obj/parse-cmd-line.o: \
		src/parse-cmd-line.cc \
		src/parse-cmd-line.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/util.o: \
		src/util.cc \
		src/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/ms-two-lock.o: \
		src/templ/ms/two-lock.cc \
		src/queue/ms/two-lock.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/ms-lock-free.o: \
		src/templ/ms/lock-free.cc \
		src/queue/ms/lock-free.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/mq-counted.o: \
		src/templ/mq/counted.cc \
		src/queue/mq/counted.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/test-ms-two-lock.o: \
		src/templ/test/ms-two-lock.cc \
		src/test/timed-throughput.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/test-ms-lock-free.o: \
		src/templ/test/ms-lock-free.cc \
		src/test/timed-throughput.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/test-mq-counted.o: \
		src/templ/test/mq-counted.cc \
		src/test/timed-throughput.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean-obj:
	-$(RM) $(OBJ)
clean-tags:
	-$(RM) */tags */types*.taghl
clean-bin:
	-$(RM) $(BIN)
clean-all: clean-obj clean-tags clean-bin
clean: clean-obj clean-bin
