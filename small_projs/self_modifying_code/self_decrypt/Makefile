CC=gcc
CFLAGS=-Wall -g
OBJECTS=cryptor.o crypt.o

TARGET=crypted
CRYPTOR=cryptor

all: $(TARGET) $(CRYPTOR)

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET): crypt.o
	$(CC) $(CFLAGS) -o $@ $^
$(CRYPTOR): cryptor.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *~ *.o $(TARGET) $(CRYPTOR)
