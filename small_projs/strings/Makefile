CC=gcc
CFLAGS=-Wall -ggdb
OBJS=validation.c search.o common.o
TARGET=string_test

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -rf *~ *.o
	rm $(TARGET)

