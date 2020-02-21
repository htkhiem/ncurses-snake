CC = g++
DEPS = utils.h snake.hpp
CFLAGS = -I. -O2 -lncursesw
OBJ = main.o snake.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
Snake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
