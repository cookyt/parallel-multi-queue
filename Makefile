OBJ = obj/multi-queue.o
INC = -Isrc/
BIN = bin/locking-test bin/multi-test
CXXFLAGS = -g

all: $(BIN)

bin/multi-test: src/multi-test.cc $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt

bin/locking-test: src/locking-test.cc $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ -lboost_thread-mt
	
obj/multi-queue.o: src/multi-queue.cc src/multi-queue.h
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean-obj:
	-$(RM) $(OBJ)
clean-tags:
	-$(RM) tags types_c.taghl
clean-bin:
	-$(RM) $(BIN)
clean-all: clean-obj clean-tags clean-bin
