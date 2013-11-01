CC=gcc
CFLAGS=-Wall -ggdb
OBJS=shape_packet.o main.o
LIBS=-lpcap
BIN=awesome_flooder

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@
$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)
clean:
	rm -f $(BIN) *.o
