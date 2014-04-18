CC = g++
CFLAGS = -std=c++11

OBJS = main.o DisjointSet.o

lab11: $(OBJS)
	$(CC) $(CFLAGS) -o lab11 $(OBJS)
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
DisjointSet.o: DisjointSet.hpp DisjointSet.cpp
	$(CC) $(CFLAGS) -c DisjointSet.cpp
.PHONY: clean
clean:
	rm -rf $(OBJS) lab11
