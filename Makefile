DEBUG=0
OBJ = obj/queue.o
INC = -Isrc/
BIN = bin/locking-test bin/multi-test
CXXFLAGS = -g -DDEBUG=$(DEBUG)

all: $(BIN)

bin/multi-test: src/multi-test.cc src/basic-test.h $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt

bin/locking-test: src/locking-test.cc src/basic-test.h $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt
	
obj/queue.o: src/queue.cc src/queue.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean-obj:
	-$(RM) $(OBJ)
clean-tags:
	-$(RM) tags types_c.taghl
clean-bin:
	-$(RM) $(BIN)
clean-all: clean-obj clean-tags clean-bin
