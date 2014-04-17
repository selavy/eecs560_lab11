CC = g++
CFLAGS = -Wall -Werror -std=c++11

OBJS = main.o

lab11: $(OBJS)
	$(CC) $(CFLAGS) -o lab11 $(OBJS)
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
.PHONY: clean
clean:
	rm -rm $(OBJS) lab11
