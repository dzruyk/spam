CC=gcc
CFLAGS=-Wall -O2 -g
TARGET=./bindump
OBJS=main.o util.o extractor.o

all: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $^ 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 
clean:
	rm -f *.o $(TARGET)
