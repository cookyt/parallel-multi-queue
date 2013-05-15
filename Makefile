DEBUG=0
OBJ = obj/util.o obj/parse-cmd-line.o \
      obj/mq-counted.o obj/ms-lock-free.o obj/ms-two-lock.o \
      obj/tests-mq-counted.o obj/tests-ms-lock-free.o obj/tests-ms-two-lock.o

INC = -Isrc/
BIN = bin/locking-test bin/multi-test bin/lock-free-test
CXXFLAGS = -std=c++11 -g -Wall -pthread -DDEBUG=$(DEBUG)

LIBBOOST = -lboost_thread -lboost_system
LIB = $(LIBBOOST) -lrt

all: $(BIN)

bin/multi-test: src/tests/mq-counted.cc obj/mq-counted.o obj/tests-mq-counted.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/bit-network-test: src/tests/mq-bit-network.cc obj/tests-ms-two-lock.o obj/ms-two-lock.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/locking-test: src/tests/ms-two-lock.cc obj/tests-ms-two-lock.o obj/ms-two-lock.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/lock-free-test: src/tests/ms-lock-free.cc obj/tests-ms-lock-free.o obj/ms-lock-free.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

obj/parse-cmd-line.o: src/parse-cmd-line.cc src/parse-cmd-line.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/util.o: src/util.cc src/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/ms-two-lock.o: src/templ/ms-two-lock.cc src/queue/ms-two-lock.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/ms-lock-free.o: src/templ/ms-lock-free.cc src/queue/ms-lock-free.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/mq-counted.o: src/templ/mq-counted.cc src/queue/mq-counted.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/tests-ms-two-lock.o: src/templ/tests-ms-two-lock.cc src/tests/test-timed-throughput.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/tests-ms-lock-free.o: src/templ/tests-ms-lock-free.cc src/tests/test-timed-throughput.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/tests-mq-counted.o: src/templ/tests-mq-counted.cc src/tests/test-timed-throughput.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean-obj:
	-$(RM) $(OBJ)
clean-tags:
	-$(RM) */tags */types*.taghl
clean-bin:
	-$(RM) $(BIN)
clean-all: clean-obj clean-tags clean-bin
clean: clean-obj clean-bin
