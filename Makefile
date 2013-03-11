DEBUG=0
OBJ = obj/queue.o obj/util.o
INC = -Isrc/
BIN = bin/locking-test bin/multi-test bin/lock-free-test
CXXFLAGS = -g -DDEBUG=$(DEBUG)

all: $(BIN)

bin/multi-test: src/tests/multi-test.cc src/tests/basic-test.h $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt

bin/locking-test: src/tests/locking-test.cc src/tests/basic-test.h $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt

bin/lock-free-test: src/tests/lock-free-test.cc src/tests/basic-test.h $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt
	
obj/queue.o: src/queue.cc src/queue.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

obj/util.o: src/util.cc src/util.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean-obj:
	-$(RM) $(OBJ)
clean-tags:
	-$(RM) */tags */types*.taghl
clean-bin:
	-$(RM) $(BIN)
clean-all: clean-obj clean-tags clean-bin
clean: clean-obj clean-bin
