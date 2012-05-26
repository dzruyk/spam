CC=gcc
CFLAGS=-Wall -ggdb

OBJECTS=common.o promisc.o list.o

TARGET=promisc

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c  $^

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o

