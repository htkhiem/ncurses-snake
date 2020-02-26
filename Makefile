CXX = g++
DEPS = utils.h snake.hpp
CFLAGS = -I. -O2 -lncurses
OBJ = main.o snake.o utils.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)
Snake: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)
