CC=gcc
CFLAGS=-Wall -ggdb

OBJECTS=common.o promisc.o print-headers.o
TARGET=promisc

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c  $^

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lpcap

clean:
	rm -f *.o

