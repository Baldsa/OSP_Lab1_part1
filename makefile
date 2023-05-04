CC = gcc
CFLAGS = -O3 -Wall -Wextra
OBJS = lab11knmN32511.o

.PHONY: all clean

all: lab11knmN32511

lab1: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

lab1.o: lab11dabN32511.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) lab11knmN32511
