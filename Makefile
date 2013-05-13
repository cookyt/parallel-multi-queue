DEBUG=0
OBJ = obj/util.o \
 	  obj/tests-two-lock.o obj/tests-lock-free.o obj/tests-multi.o \
	  obj/queue-two-lock.o obj/queue-lock-free.o obj/queue-multi.o
INC = -Isrc/
BIN = bin/locking-test bin/multi-test bin/lock-free-test
CXXFLAGS = -g -Wall -pthread -DDEBUG=$(DEBUG)

LIBBOOST = -lboost_thread -lboost_system
LIB = $(LIBBOOST) -lrt

all: $(BIN)

bin/multi-test: src/tests/multi-test.cc obj/tests-multi.o obj/queue-multi.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/bit-network-test: src/tests/mq-bit-network-test.cc obj/tests-two-lock.o obj/queue-two-lock.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lrt -lpthread -std=c++11

bin/locking-test: src/tests/locking-test.cc obj/tests-two-lock.o obj/queue-two-lock.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

bin/lock-free-test: src/tests/lock-free-test.cc obj/tests-lock-free.o obj/queue-lock-free.o obj/util.o obj/parse-cmd-line.o
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LIB)

obj/parse-cmd-line.o: src/tests/parse-cmd-line.cc src/tests/parse-cmd-line.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/util.o: src/util.cc src/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/queue-two-lock.o: src/templ/queue-two-lock.cc src/queue/ms-two-lock-queue.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/queue-lock-free.o: src/templ/queue-lock-free.cc src/queue/ms-lock-free-queue.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/queue-multi.o: src/templ/queue-multi.cc src/queue/multi-queue.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/tests-two-lock.o: src/templ/tests-two-lock.cc src/tests/basic-test.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/tests-lock-free.o: src/templ/tests-lock-free.cc src/tests/basic-test.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/tests-multi.o: src/templ/tests-multi.cc src/tests/basic-test.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean-obj:
	-$(RM) $(OBJ)
clean-tags:
	-$(RM) */tags */types*.taghl
clean-bin:
	-$(RM) $(BIN)
clean-all: clean-obj clean-tags clean-bin
clean: clean-obj clean-bin
