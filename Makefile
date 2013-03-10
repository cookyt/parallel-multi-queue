OBJ = obj/multi-queue.o
INC = -Isrc/
BIN = bin/locking-queue-test
CXXFLAGS = -g

all: $(BIN)

bin/locking-queue-test: src/locking-queue-test.cc $(OBJ)
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
