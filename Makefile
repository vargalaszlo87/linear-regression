CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LDLIBS = -lm

TARGET = linr
OBJS = main.o linr.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDLIBS)

main.o: main.c linr.h
	$(CC) $(CFLAGS) -c main.c -o main.o

linr.o: linr.c linr.h
	$(CC) $(CFLAGS) -c linr.c -o linr.o

debug: CFLAGS += -g -O0
debug: clean $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all debug clean
